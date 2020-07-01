#include "DifferentialGeometry.h"


DifferentialGeometry::DifferentialGeometry():
    u(0.), v(0.), shape(0)
{

}

DifferentialGeometry::DifferentialGeometry(
    const vec3d& point,
    double u, double v,
    const vec3d& dpdu, const vec3d& dpdv,
    const vec3d& normal,
    const ShapeRT* sh, bool isFront):
    point(point),
    u(u), v(v),
    dpdu(dpdu), dpdv(dpdv),
    normal(normal),
    shape(sh), isFront(isFront)
{

}
