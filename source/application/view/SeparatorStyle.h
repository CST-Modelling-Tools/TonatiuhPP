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
    bool showMesh;

protected:
    SoSwitch* sWires;
    SoDrawStyle* wireStyle;

   // SoNode interface
public:
   void GLRender(SoGLRenderAction* action);
};

