#include "SeparatorStyle.h"

#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoPolygonOffset.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoLightModel.h>

SO_NODE_SOURCE(SeparatorStyle)


void SeparatorStyle::initClass()
{
    SO_NODE_INIT_CLASS(SeparatorStyle, SoNode, "Node");
}

SeparatorStyle::SeparatorStyle()
{
    SO_NODE_CONSTRUCTOR(SeparatorStyle);

    m_root = new SoSeparator;
    m_root->ref();
    sWires = new SoSwitch;
    sWires->whichChild = SO_SWITCH_ALL;

    SoBaseColor* bc = new SoBaseColor;
    bc->rgb.setValue(0, 0.5, 0.5);
    bc->setOverride(TRUE);
    sWires->addChild(bc);

//    SoMaterialBinding* mb = new SoMaterialBinding;
//    mb->setOverride(TRUE);
//    mb->value = SoMaterialBinding::OVERALL;
//    sWires->addChild(mb);

    wireStyle = new SoDrawStyle;
    wireStyle->style = SoDrawStyle::LINES;
    wireStyle->setOverride(TRUE);
    sWires->addChild(wireStyle);

    SoLightModel* lightmodel = new SoLightModel;
    lightmodel->model = SoLightModel::BASE_COLOR;
    lightmodel->setOverride(TRUE);
    sWires->addChild(lightmodel);

//    SoPolygonOffset* po = new SoPolygonOffset;
//    po->styles = SoPolygonOffset::LINES;
//    po->factor = -1.; // useless for lines?
//    po->units = -1.;
//    sWires->addChild(po);

    m_root->addChild(sWires);
    showMesh = false;
}

SeparatorStyle::~SeparatorStyle()
{
    m_root->unref();
}

#include <Inventor/nodes/SoDepthBuffer.h>
#include <Inventor/actions/SoGLRenderAction.h>

void SeparatorStyle::GLRender(SoGLRenderAction* action)
{
    if (!showMesh) return;

    SoState* state = action->getState();
    state->push();
//    wireStyle->style = SoDrawStyle::LINES;
//    sWires->whichChild = SO_SWITCH_NONE;
//    m_root->getChild(1)->GLRender(action);

    SbBool test_out, write_out;
    SoDepthBufferElement::DepthWriteFunction function_out;
    SbVec2f range_out;
    SoDepthBufferElement::get(state, test_out, write_out, function_out, range_out);
//    range_out.setValue(0.f, 1.0f);
    write_out = FALSE;
//    function_out = SoDepthBufferElement::NEVER;
    SoDepthBufferElement::set(state, test_out, write_out, function_out, range_out);


//    wireStyle->style = SoDrawStyle::FILLED;
//    sWires->whichChild = SO_SWITCH_ALL;
    m_root->GLRender(action);

//    SoSeparator::GLRender(action);
//    SoSeparator::GLRender(action);

//    getChild(1)->GLRender(action);
      state->pop();
}
