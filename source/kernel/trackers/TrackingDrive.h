#pragma once

#include "kernel/TonatiuhKernel.h"

#include "libraries/math/2D/vec2d.h"
#include "libraries/math/3D/vec3d.h"
#include "libraries/math/3D/Transform.h"
#include "libraries/math/1D/Interval.h"
#include "libraries/math/1D/IntervalPeriodic.h"


struct TONATIUH_KERNEL TrackingDrive
{
    TrackingDrive(const vec3d& shift, const vec3d& axis, const IntervalPeriodic& angles);

    vec3d shift;
    vec3d axis;
    IntervalPeriodic angles;

    Transform getTransform(double angle) const;
};


struct TONATIUH_KERNEL TrackingVertex
{
    TrackingVertex(const vec3d& shift, const vec3d& normal);

    vec3d shift;
    vec3d normal;
};
