#pragma once

#include <Inventor/nodes/SoSubNode.h>

class SoSeparator;
class SoPerspectiveCamera;
class QString;


// a background which rotates with camera but does not move
class SkyBackground: public SoNode
{
    SO_NODE_HEADER(SkyBackground);

public:
    static void initClass();
    SkyBackground();

    void GLRender(SoGLRenderAction* action);

protected:
    SoSeparator* m_root;
    SoPerspectiveCamera* m_camera;

protected:
    ~SkyBackground();

    SoSeparator* makeSky();
    SoSeparator* makeLabels();
    void makeLabelAE(SoSeparator* parent, double azimuth, double elevation, const QString& text);
};


