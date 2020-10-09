#pragma once

#include <Inventor/nodes/SoSeparator.h>

#include <Inventor/nodes/SoSubNode.h>
class SoSwitch;
class SoDrawStyle;


class SeparatorStyle: public SoNode
{
   SO_NODE_HEADER(SeparatorStyle);

public:
   static void initClass();
   SeparatorStyle();
   ~SeparatorStyle();

    SoSeparator* m_root;
    SoSwitch* sWires;
    SoDrawStyle* wireStyle;

    bool showMesh;
   // SoNode interface
public:
   void GLRender(SoGLRenderAction* action);
};

