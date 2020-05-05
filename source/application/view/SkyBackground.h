#pragma once

#include <Inventor/nodes/SoSubNode.h>

class SoSeparator;
class SoPerspectiveCamera;


// a background which rotates with camera but does not move
class SkyBackground: public SoNode
{
    SO_NODE_HEADER(SkyBackground);

public:
    static void initClass();
    SkyBackground();

    SoSeparator* m_root;
    SoPerspectiveCamera* m_camera;
    SoSeparator* m_sky;

    void GLRender(SoGLRenderAction* action);

protected:
    ~SkyBackground();
    void addSky();
};
