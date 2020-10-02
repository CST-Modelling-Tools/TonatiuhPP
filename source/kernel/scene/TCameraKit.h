#pragma once
#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodekits/SoCameraKit.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>


#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFVec3f.h>
#include "kernel/scene/TSFRotation.h"


class TONATIUH_KERNEL TCameraKit: public SoBaseKit
{
    SO_KIT_HEADER(TCameraKit);
    SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);

public:
    static void initClass();
    TCameraKit();

    SoSFVec3f position;
    TSFRotation rotation;
    SoSFBool perspective;
    SoSFDouble angle; // full vertical for full screen

    SoCameraKit* m_cameraKit;
//    SoPerspectiveCamera* m_camera;
};

