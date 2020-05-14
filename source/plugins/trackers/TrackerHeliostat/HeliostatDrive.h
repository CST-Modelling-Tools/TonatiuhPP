#pragma once

#include "libraries/geometry/Vector2D.h"
#include "libraries/geometry/Vector3D.h"
#include "libraries/geometry/Transform.h"
#include "libraries/geometry/Interval.h"

struct HeliostatDrive
{
    HeliostatDrive(const Vector3D& shift, const Vector3D& axis, const IntervalAngular& angles);

    Vector3D shift;
    Vector3D axis;
    IntervalAngular angles;

    Transform getTransform(double angle) const;
};


struct Vertex3D
{
    Vertex3D(const Vector3D& point, const Vector3D& normal);

    Vector3D point;
    Vector3D normal;
};


