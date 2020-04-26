#include "TShape.h"

#include "libraries/geometry/Point3D.h"

SO_NODE_ABSTRACT_SOURCE(TShape)


void TShape::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(TShape, SoShape, "Shape");
}

Point3D TShape::Sample(double u, double v) const
{
    return GetPoint3D(u, v);
}

bool TShape::OutOfRange(double u, double v) const
{
    return u < 0. || u > 1. || v < 0. || v > 1.;
}
