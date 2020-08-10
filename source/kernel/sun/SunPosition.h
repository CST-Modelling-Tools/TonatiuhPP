#pragma once
#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFDouble.h>
#include "libraries/math/3D/vec3d.h"

class SoNodeSensor;
class SoSensor;
class SoTransform;


class TONATIUH_KERNEL SunPosition: public SoNode
{
    SO_NODE_HEADER(SunPosition);

public:
    static void initClass();
    SunPosition();
    ~SunPosition();

    SoSFDouble azimuth;  // in degrees
    SoSFDouble elevation;
    SoSFDouble irradiance;

    vec3d getSunVector();

    SoSeparator* getRoot() const {return m_root;}

protected:
    SoSeparator* m_root;
    SoTransform* m_transform;
    void create();

    SoNodeSensor* m_sensor;
    static void update(void* data, SoSensor*);
};

