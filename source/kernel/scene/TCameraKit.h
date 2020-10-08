#pragma once
#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodekits/SoCameraKit.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFVec2f.h>
#include "kernel/scene/TSFRotation.h"

class SoSensor;
class SoNodeSensor;
class SoFieldSensor;
class TPerspectiveCamera;

class TONATIUH_KERNEL TCameraKit: public SoBaseKit
{
    SO_KIT_HEADER(TCameraKit);
    SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);

public:
    static void initClass();
    TCameraKit();
    ~TCameraKit();

    SoSFVec3f position;
    SoSFVec2f rotation;
    SoSFBool perspective;
    SoSFDouble angle; // full vertical for full screen

    void setCamera(TPerspectiveCamera* camera);
    TPerspectiveCamera* m_camera;

protected:
    SoNodeSensor* m_sensor;
    SoNodeSensor* m_sensorPR;
//    SoFieldSensor* m_sensor_pos;
//    SoFieldSensor* m_sensor_rot;
    static void update(void* data, SoSensor*);
    static void updatePR(void* data, SoSensor*);
    void sync();
    void syncPR();
};

