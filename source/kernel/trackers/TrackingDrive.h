#pragma once

#include "kernel/TonatiuhKernel.h"

#include "libraries/math/Vector2D.h"
#include "libraries/math/Vector3D.h"
#include "libraries/math/Transform.h"
#include "libraries/math/Interval.h"


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
