#include "ApertureRectangle.h"

#include "kernel/random/Random.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/Ray.h"


SO_NODE_SOURCE(ApertureRectangle)

void ApertureRectangle::initClass()
{
     SO_NODE_INIT_CLASS(ApertureRectangle, Aperture, "Aperture");
}

ApertureRectangle::ApertureRectangle()
{
    SO_NODE_CONSTRUCTOR(ApertureRectangle);
}

bool ApertureRectangle::OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& rand, Ray& rayOut) const
{
    Q_UNUSED(rayIn)
    Q_UNUSED(dg)
    Q_UNUSED(rand)
    Q_UNUSED(rayOut)

    return false;
}
