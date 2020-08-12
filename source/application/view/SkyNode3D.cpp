#include "SkyNode3D.h"

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
#include "libraries/math/gcf.h"

SO_NODE_SOURCE(SkyNode3D)


// based on Preetham model
struct SkyGradient
{
   QColor cH; // horizon
   QColor cZ; // zenith
   QVector3D g; // transition

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
       int r = gcf::lerp(cH.red(), cZ.red(), m.x());
       int g = gcf::lerp(cH.green(), cZ.green(), m.y());
       int b = gcf::lerp(cH.blue(), cZ.blue(), m.z());
       return QColor(r, g, b);
   }
};


void SkyNode3D::initClass()
{
    SO_NODE_INIT_CLASS(SkyNode3D, SoNode, "Node");
}

SkyNode3D::SkyNode3D(void)
{
    SO_NODE_CONSTRUCTOR(SkyNode3D);

    m_root = new SoSeparator;
    m_root->ref();

    m_camera = new SoPerspectiveCamera;
    m_camera->position = SbVec3f(0., 0., 0.);
//    m_camera->focalDistance = 0.5f;
    m_camera->nearDistance = 0.1;
    m_camera->farDistance = 10.f;
    m_camera->heightAngle = 30.*gcf::degree; // 30 deg
    m_camera->enableNotify(FALSE);
    m_root->addChild(m_camera);

    SoLightModel* lightmodel = new SoLightModel;
    lightmodel->model.setValue(SoLightModel::BASE_COLOR);
    m_root->addChild(lightmodel);

    m_root->addChild(makeSky());
    m_root->addChild(makeLabels());
}

SkyNode3D::~SkyNode3D()
{
    m_root->unref();
}

SoSeparator* SkyNode3D::makeSky()
{
    SoSeparator* ans = new SoSeparator;
//    ans->renderCulling = SoSeparator::OFF;

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
            float r = alpha >= 0. ? 1.2 : 0.8; // for sun set
            vertices << r*SbVec3f(x, y, z);

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

SoSeparator* SkyNode3D::makeLabels()
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

void SkyNode3D::makeLabelAE(SoSeparator* parent, double azimuth, double elevation, const QString& text)
{
    SoSeparator* ans = new SoSeparator;

    // Rz(-gamma) Rx(alpha)
    SoTransform* sTransform = new SoTransform;
    sTransform->rotation =
        SbRotation(SbVec3f(1., 0., 0.), (90. + elevation)*gcf::degree) *
        SbRotation(SbVec3f(0., 0., 1.), -azimuth*gcf::degree);
    ans->addChild(sTransform);

    sTransform = new SoTransform;
    sTransform->translation = SbVec3f(0., 0., -1.);
    ans->addChild(sTransform);

    SoText3* sText = new SoText3;
    sText->string = text.toLatin1().data();
    sText->justification = SoText3::CENTER;
    ans->addChild(sText);

    parent->addChild(ans);
}

#include <Inventor/nodes/SoDepthBuffer.h>
void SkyNode3D::GLRender(SoGLRenderAction* action)
{
  SoState* state = action->getState();
  state->push();

  SoModelMatrixElement::makeIdentity(state, this);
  const SbMatrix& tmp = SoViewingMatrixElement::get(state);
  m_camera->orientation = SbRotation(tmp).inverse();

  SbBool test_out, write_out;
  SoDepthBufferElement::DepthWriteFunction function_out;
  SbVec2f range_out;
  SoDepthBufferElement::get(state, test_out, write_out, function_out, range_out);
  range_out[0] = 0.99999f;
  range_out[1] = 1.0f;
  SoDepthBufferElement::set(state, test_out, write_out, function_out, range_out);

  m_root->GLRender(action);
//  glClear(GL_DEPTH_BUFFER_BIT);

  state->pop();
}
