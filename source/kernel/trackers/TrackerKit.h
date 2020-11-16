#pragma once

#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/fields/SoSFString.h>


class TSeparatorKit;
class Transform;
struct vec3d;
class SoFieldSensor;
class SoSensor;

class TONATIUH_KERNEL TrackerKit: public SoBaseKit
{
    SO_KIT_HEADER(TrackerKit);

    SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);

public:
    static void initClass();
    TrackerKit();

    SoSFBool enabled;
    SoSFNode armature;
    SoSFString shape;
    SoSFNode target;

    void update(TSeparatorKit* parent, const Transform& toGlobal, const vec3d& vSun);

    TSeparatorKit* m_parent;
    SoShapeKit* m_shapeKit;

protected:
    ~TrackerKit();

    void setDefaultOnNonWritingFields();

    SoFieldSensor* m_sensor_shape;
    static void onSensor_shape(void* data, SoSensor*);

    SoFieldSensor* m_sensor_target;
    static void onSensor_target(void* data, SoSensor*);
};

