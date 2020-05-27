#include "MaterialAbsorber.h"

#include "kernel/random/Random.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/Ray.h"


SO_NODE_SOURCE(MaterialAbsorber)

void MaterialAbsorber::initClass()
{
     SO_NODE_INIT_CLASS(MaterialAbsorber, MaterialRT, "MaterialRT");
}

MaterialAbsorber::MaterialAbsorber()
{
    SO_NODE_CONSTRUCTOR(MaterialAbsorber);
}

bool MaterialAbsorber::OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& rand, Ray& rayOut) const
{
    Q_UNUSED(rayIn)
    Q_UNUSED(dg)
    Q_UNUSED(rand)
    Q_UNUSED(rayOut)

    return false;
}
