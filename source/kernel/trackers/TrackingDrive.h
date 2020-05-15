#pragma once

#include "kernel/TonatiuhKernel.h"

#include "libraries/geometry/Vector2D.h"
#include "libraries/geometry/Vector3D.h"
#include "libraries/geometry/Transform.h"
#include "libraries/geometry/Interval.h"


struct TONATIUH_KERNEL TrackingDrive
{
    TrackingDrive(const Vector3D& shift, const Vector3D& axis, const IntervalAngular& angles);

    Vector3D shift;
    Vector3D axis;
    IntervalAngular angles;

    Transform getTransform(double angle) const;
};


struct TONATIUH_KERNEL TrackingVertex
{
    TrackingVertex(const Vector3D& shift, const Vector3D& normal);

    Vector3D shift;
    Vector3D normal;
};
