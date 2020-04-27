#pragma once

#include "kernel/TonatiuhKernel.h"
#include "libraries/geometry/Vector3D.h"

class TShape;

struct TONATIUH_KERNEL DifferentialGeometry
{
    DifferentialGeometry();
    DifferentialGeometry(
        const Point3D& point,
        double u, double v,
        const Vector3D& dpdu, const Vector3D& dpdv,
        const NormalVector& normal,
        const TShape* shape);

    Point3D point;
    double u;
    double v;
    Vector3D dpdu;
    Vector3D dpdv;
    NormalVector normal;

    const TShape* shape;
    bool shapeFrontSide;
};
