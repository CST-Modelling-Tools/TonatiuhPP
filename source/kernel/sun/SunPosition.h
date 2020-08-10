#pragma once
#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFDouble.h>
#include "libraries/math/3D/vec3d.h"

class TONATIUH_KERNEL SunPosition: public SoNode
{
    SO_NODE_HEADER(SunPosition);

public:
    static void initClass();
    SunPosition();

    vec3d getSunVector();
    SoSFDouble azimuth;  // in radians
    SoSFDouble elevation;
    SoSFDouble irradiance;
};

