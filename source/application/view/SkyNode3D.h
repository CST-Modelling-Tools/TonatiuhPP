#pragma once

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoSeparator.h>

class SoSeparator;
class SoPerspectiveCamera;
class QString;


// a background which rotates with camera but does not move
class SkyNode3D: public SoNode
{
    SO_NODE_HEADER(SkyNode3D);

public:
    static void initClass();
    SkyNode3D();

    SoSeparator* getRoot() {return m_root;}
    void GLRender(SoGLRenderAction* action);
    void updateSkyCamera(SoPerspectiveCamera* camera);

protected:
    SoSeparator* m_root;
    SoPerspectiveCamera* m_camera;

protected:
    ~SkyNode3D();

    SoSeparator* makeSky();
    SoSeparator* makeLabels();
    void makeLabelAE(SoSeparator* parent, double azimuth, double elevation, const QString& text);

    // SoBase interface
//public:
//    void getBoundingBox(SoGetBoundingBoxAction* action);
};


