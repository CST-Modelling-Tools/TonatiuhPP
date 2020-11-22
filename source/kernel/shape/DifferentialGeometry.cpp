#include "DifferentialGeometry.h"


DifferentialGeometry::DifferentialGeometry():
    uv(vec2d::Zero), shape(0)
{

}

DifferentialGeometry::DifferentialGeometry(
    const vec3d& point,
    double u, double v,
    const vec3d& dpdu, const vec3d& dpdv,
    const vec3d& normal,
    const ShapeRT* sh, bool isFront):
    point(point),
    dpdu(dpdu), dpdv(dpdv),
    normal(normal),
    uv(u, v),
    shape(sh), isFront(isFront)
{

}
