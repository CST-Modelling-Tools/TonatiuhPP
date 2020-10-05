#pragma once

#include <Inventor/nodes/SoSubNode.h>

class SoSeparator;
class SoPerspectiveCamera;
class QString;

// an overlay which rotates with camera but does not move
class OverlayNode: public SoNode
{
    SO_NODE_HEADER(OverlayNode);

public:
    static void initClass();
    OverlayNode();

    SoSeparator* getRoot() {return m_root;}
    void GLRender(SoGLRenderAction* action);
    void updateSkyCamera(SoPerspectiveCamera* camera);

    void showAim(bool on);

protected:
    SoSeparator* m_root;
    SoPerspectiveCamera* m_camera;

protected:
    ~OverlayNode();
};

