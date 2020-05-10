#pragma once

#include "kernel/TonatiuhKernel.h"
#include "libraries/geometry/Vector3D.h"

class ShapeAbstract;

struct TONATIUH_KERNEL DifferentialGeometry
{
    DifferentialGeometry();
    DifferentialGeometry(const Point3D& point,
        double u, double v,
        const Vector3D& dpdu, const Vector3D& dpdv,
        const Vector3D& normal,
        const ShapeAbstract* shape, bool isFront);

    Point3D point;
    double u;
    double v;
    Vector3D dpdu;
    Vector3D dpdv;
    Vector3D normal;

    const ShapeAbstract* shape;
    bool isFront;
};
