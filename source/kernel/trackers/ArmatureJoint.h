#pragma once

#include "kernel/TonatiuhKernel.h"

#include "libraries/math/3D/Transform.h"
#include "libraries/math/1D/IntervalPeriodic.h"
#include "libraries/math/gcf.h"


struct TONATIUH_KERNEL ArmatureJoint
{
    ArmatureJoint(
        const vec3d& shift = vec3d::Zero,
        const vec3d& axis = vec3d::UnitZ,
        const IntervalPeriodic& angles = IntervalPeriodic(-90.*gcf::degree, 90.*gcf::degree));

    vec3d shift;
    vec3d axis;
    IntervalPeriodic angles;

    Transform getTransform(double angle) const;
};


struct TONATIUH_KERNEL ArmatureVertex
{
    ArmatureVertex(
        const vec3d& shift = vec3d::Zero,
        const vec3d& normal = vec3d::UnitZ
    );

    vec3d shift;
    vec3d normal;
};
