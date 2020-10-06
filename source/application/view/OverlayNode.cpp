#include "OverlayNode.h"

#include <Inventor/system/gl.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/actions/SoSearchAction.h>

#include <QVector>

SO_NODE_SOURCE(OverlayNode)


void OverlayNode::initClass()
{
    SO_NODE_INIT_CLASS(OverlayNode, SoNode, "Node");
}

OverlayNode::OverlayNode()
{
    SO_NODE_CONSTRUCTOR(OverlayNode);

    m_root = new SoSeparator;
    m_root->ref();

    m_camera = new SoPerspectiveCamera;
    m_camera->position = SbVec3f(0., 0., 1.);
    m_camera->nearDistance = 0.1;
    m_camera->farDistance = 10.f;
    m_camera->enableNotify(FALSE);
    m_root->addChild(m_camera);

    SoLightModel* lightmodel = new SoLightModel;
    lightmodel->model.setValue(SoLightModel::BASE_COLOR);
    m_root->addChild(lightmodel);


    SoSwitch* sAim = new SoSwitch;
    sAim->setName("aim");
    m_root->addChild(sAim);

    SoSeparator* sep = new SoSeparator;
    sAim->addChild(sep);

    SoMaterial *myMaterial = new SoMaterial;
    myMaterial->diffuseColor = SbVec3f(1., 1., 1.);
    myMaterial->transparency = 0.5; // corners
    sep->addChild(myMaterial);

//    SoFont *myFont = new SoFont;
//    myFont->name.setValue("Times-Roman");
//    myFont->size.setValue(24.0);
//    sep->addChild(myFont);

//    SoText2* text = new SoText2;
//    text->string = "XVXV";
//    text->justification = SoText2::CENTER;
//    sep->addChild(text);

    SoDrawStyle* sStyle = new SoDrawStyle;
    sStyle->lineWidth = 2;
//    sStyle->pointSize = 10;
//    sStyle->style = SoDrawStyle::FILLED;
    sep->addChild(sStyle);

    float scale = 0.0007;
    float w = 64/2*scale;
    float h = 48/2*scale;
    float s = 12*scale;
    float pts[][3] = {
        {w, h - s, 0},
        {w, h, 0},
        {w - s, h, 0},

        {-w + s, h, 0},
        {-w, h, 0},
        {-w, h - s, 0},

        {-w, -h + s, 0},
        {-w, -h, 0},
        {-w + s, -h, 0},

        {w - s, -h, 0},
        {w, -h, 0},
        {w, -h + s, 0}
    };

    int szs[] = {3, 3, 3, 3};

    SoCoordinate3* sPoints = new SoCoordinate3;
    sPoints->point.setValues(0, 12, pts);
    sep->addChild(sPoints);

    SoLineSet* sLines = new SoLineSet;
    sLines->numVertices.setValues(0, 4, szs);
    sep->addChild(sLines);
}

OverlayNode::~OverlayNode()
{
    m_root->unref();
}

#include <Inventor/nodes/SoDepthBuffer.h>
void OverlayNode::GLRender(SoGLRenderAction* action)
{
  SoState* state = action->getState();
  state->push();

//  SoModelMatrixElement::makeIdentity(state, this);
//  const SbMatrix& tmp = SoViewingMatrixElement::get(state);
//  m_camera->orientation = SbRotation(tmp).inverse();

//  SbBool test_out, write_out;
//  SoDepthBufferElement::DepthWriteFunction function_out;
//  SbVec2f range_out;
//  SoDepthBufferElement::get(state, test_out, write_out, function_out, range_out);
//  range_out[0] = 0.00000f;
//  range_out[1] = 0.00001f;
//  SoDepthBufferElement::set(state, test_out, write_out, function_out, range_out);
  glClear(GL_DEPTH_BUFFER_BIT);

  m_root->GLRender(action);

  state->pop();
}

void OverlayNode::updateSkyCamera(SoPerspectiveCamera* camera)
{
    m_camera->heightAngle = camera->heightAngle.getValue();
}

void OverlayNode::showAim(bool on)
{
    SoSearchAction search;
    search.setName("aim");
    search.apply(m_root);
    SoPath* path = search.getPath();
    if (!path) return;
    SoSwitch* group = (SoSwitch*) path->getTail();
    group->whichChild = on ? SO_SWITCH_ALL : SO_SWITCH_NONE;
}
