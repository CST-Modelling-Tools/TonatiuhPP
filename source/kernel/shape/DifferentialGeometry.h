#pragma once

#include "kernel/TonatiuhKernel.h"
#include "libraries/math/3D/vec3d.h"

class ShapeRT;

struct TONATIUH_KERNEL DifferentialGeometry
{
    DifferentialGeometry();
    DifferentialGeometry(const vec3d& point,
        double u, double v,
        const vec3d& dpdu, const vec3d& dpdv,
        const vec3d& normal,
        const ShapeRT* shape, bool isFront);

    vec3d point;
    double u;
    double v;
    vec3d dpdu; // not normalized
    vec3d dpdv; // not normalized
    vec3d normal; // normalized

    const ShapeRT* shape;
    bool isFront;
};
