#include "SkyBackground.h"

#include <Inventor/SbRotation.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoQuadMesh.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/system/gl.h>

#include <QColor>
#include <QVector3D>


SO_NODE_SOURCE(SkyBackground)


// based on Preetham model
struct SkyGradient
{
   QColor cH;
   QColor cZ;
   QVector3D g;

   QVector3D mix(double alpha)
   {
       alpha /= M_PI/2;
       if (alpha < 0.001) alpha = 0.001;
       else if (alpha >= 0.999) alpha = 0.999;
       alpha = 1. - alpha;
       double temp = alpha*alpha;
       temp = temp/(temp - 1.);
       double x = exp(g.x()*temp);
       double y = exp(g.y()*temp);
       double z = exp(g.z()*temp);
       return QVector3D(x, y, z);
   }

   QColor find(double theta) {
       QVector3D m = mix(theta);
       int r = m.x()*cZ.red() + (1. - m.x())*cH.red();
       int g = m.y()*cZ.green() + (1. - m.y())*cH.green();
       int b = m.z()*cZ.blue() + (1. - m.z())*cH.blue();
       return QColor(r, g, b);
   }
};


void SkyBackground::initClass()
{
    SO_NODE_INIT_CLASS(SkyBackground, SoNode, "Node");
}

SkyBackground::SkyBackground(void)
{
    SO_NODE_CONSTRUCTOR(SkyBackground);

    m_root = new SoSeparator;
    m_root->ref();

    m_camera = new SoPerspectiveCamera;
    m_camera->position = SbVec3f(0., 0., 0.);
    m_camera->nearDistance = 0.1f;
    m_camera->farDistance = 10.f;
    m_camera->heightAngle = 0.523599;
    m_camera->enableNotify(FALSE);
    m_root->addChild(m_camera);

    SoLightModel* lightmodel = new SoLightModel;
    lightmodel->model.setValue(SoLightModel::BASE_COLOR);
    m_root->addChild(lightmodel);

    m_sky = makeSky();
    m_root->addChild(m_sky);
    m_root->addChild(makeLabels());
}

SkyBackground::~SkyBackground()
{
    m_root->unref();
}

SoSeparator* SkyBackground::makeSky()
{
    SoSeparator* ans = new SoSeparator;

    SkyGradient grSky;
    grSky.cH = QColor("#b2c3d2");
    grSky.cZ = QColor("#60769d");
    grSky.g = QVector3D(0.2, 0.2, 0.2);

    SkyGradient grGround;
    grGround.cH = QColor("#e0ddd0");
    grGround.cZ = QColor("#e6e3d1"); // c9c7be
    grGround.g = QVector3D(0.1, 0.1, 0.1);

    int nMax = 24; // azimuth
    QVector<float> phis;
    for (int n = 0; n < nMax; ++n)
        phis << 2.*M_PI*n/(nMax - 1);

    float deg = M_PI/180;
    QVector<float> alphas = {
        -90.f, -60.f, -30.f, -10.f, -5.f, -2.5f, -0.01f,
        0.01f, 2.5f, 5.f, 10.f, 30.f, 60.f, 90.f};
    int mMax = alphas.size(); // elevation
    for (float& alpha : alphas)
        alpha *= deg;

    QVector<SbVec3f> vertices;
    QVector<SbColor> colors;
    QColor c;
    for (float phi : phis)
    {
        for (float alpha : alphas)
        {
            float x = cos(phi)*cos(alpha);
            float y = sin(phi)*cos(alpha);
            float z = sin(alpha);
            vertices << SbVec3f(x, y, z);

            if (alpha > 0.)
                c = grSky.find(alpha);
            else
                c = grGround.find(-alpha);
            colors << SbColor(c.redF(), c.greenF(), c.blueF());
        }
    }

    SoCoordinate3* coords = new SoCoordinate3;
    coords->point.setValues(0, vertices.size(), vertices.data());
    ans->addChild(coords);

    SoMaterial* material = new SoMaterial;
    material->diffuseColor.setValues(0, colors.size(), colors.data());
    ans->addChild(material);

    SoMaterialBinding* binding = new SoMaterialBinding;
    binding->value = SoMaterialBinding::PER_VERTEX;
    ans->addChild(binding);

    SoQuadMesh* mesh = new SoQuadMesh;
    mesh->verticesPerRow = mMax;
    mesh->verticesPerColumn = nMax;
    ans->addChild(mesh);

    return ans;
}

SoSeparator* SkyBackground::makeLabels()
{
    SoSeparator* ans = new SoSeparator;

    SoFont* font = new SoFont;
    font->name = "Arial:Bold";
    font->size = 0.010;
    ans->addChild(font);

    SoMaterial* material = new SoMaterial;
    material->diffuseColor = SbColor(1., 1., 1.);
    material->transparency = 0.5;
    ans->addChild(material);

    makeLabelAE(ans, 0., 0.5, "North");
    makeLabelAE(ans, 90., 0.5, "East");
    makeLabelAE(ans, 180., 0.5, "South");
    makeLabelAE(ans, 270., 0.5, "West");

    return ans;
}

#include "libraries/geometry/gcf.h"
void SkyBackground::makeLabelAE(SoSeparator* parent, double azimuth, double elevation, const QString& text)
{
    SoSeparator* ans = new SoSeparator;

    // Rz(-gamma) Rx(alpha)
    SoTransform* sTransform = new SoTransform;
    sTransform->rotation =
        SbRotation(SbVec3f(1., 0., 0.), elevation*gcf::degree) *
        SbRotation(SbVec3f(0., 0., 1.), -azimuth*gcf::degree);
    ans->addChild(sTransform);

    // from {0, 1, 0}
    sTransform = new SoTransform;
    sTransform->translation = SbVec3f(0., 0.8, 0.);
    sTransform->rotation = SbRotation(SbVec3f(1., 0., 0.), 90.*gcf::degree);
    ans->addChild(sTransform);

    SoText3* sText = new SoText3;
    sText->string = text.toLatin1().data();
    sText->justification = SoText3::CENTER;
    ans->addChild(sText);

    parent->addChild(ans);
}

void SkyBackground::GLRender(SoGLRenderAction* action)
{
  SoState* state = action->getState();
  state->push();

  const SbMatrix& tmp = SoViewingMatrixElement::get(state);
  SbRotation rot(tmp);
  m_camera->orientation = rot.inverse();
  SoModelMatrixElement::makeIdentity(state, this);

  m_root->GLRender(action);

  glClear(GL_DEPTH_BUFFER_BIT);
  state->pop();
}
