#pragma once

#include "kernel/TonatiuhKernel.h"
#include "libraries/math/Vector3D.h"

class ShapeRT;

struct TONATIUH_KERNEL DifferentialGeometry
{
    DifferentialGeometry();
    DifferentialGeometry(const Vector3D& point,
        double u, double v,
        const Vector3D& dpdu, const Vector3D& dpdv,
        const Vector3D& normal,
        const ShapeRT* shape, bool isFront);

    Vector3D point;
    double u;
    double v;
    Vector3D dpdu; // not normalized
    Vector3D dpdv; // not normalized
    Vector3D normal; // normalized

    const ShapeRT* shape;
    bool isFront;
};
