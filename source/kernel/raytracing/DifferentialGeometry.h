#pragma once

#include "kernel/TonatiuhKernel.h"
#include "libraries/geometry/NormalVector.h"
#include "libraries/geometry/Point3D.h"
#include "libraries/geometry/Vector3D.h"

class TShape;

struct TONATIUH_KERNEL DifferentialGeometry
{
    DifferentialGeometry();
    DifferentialGeometry(const Point3D& P, const Vector3D& DPDU,
                         const Vector3D& DPDV, const Vector3D& DNDU,
                         const Vector3D& DNDV, double uu, double vv,
                         const TShape* sh);
    ~DifferentialGeometry();

    Point3D point;
    NormalVector normal;
    double u;
    double v;
    double dudx;
    double dvdx;
    double dudy;
    double dvdy;
    const TShape* pShape;
    Vector3D dpdu;
    Vector3D dpdv;
    Vector3D dndu;
    Vector3D dndv;
    bool shapeFrontSide;
};
