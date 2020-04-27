#include "ShapeAbstract.h"

#include "libraries/geometry/Point3D.h"

SO_NODE_ABSTRACT_SOURCE(ShapeAbstract)


void ShapeAbstract::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(ShapeAbstract, SoShape, "Shape");
}

Point3D ShapeAbstract::Sample(double u, double v) const
{
    return GetPoint3D(u, v);
}

bool ShapeAbstract::OutOfRange(double u, double v) const
{
    return u < 0. || u > 1. || v < 0. || v > 1.;
}
