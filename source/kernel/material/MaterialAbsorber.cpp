#include "MaterialAbsorber.h"

#include "libraries/math/3D/Ray.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "kernel/random/Random.h"


SO_NODE_SOURCE(MaterialAbsorber)

void MaterialAbsorber::initClass()
{
     SO_NODE_INIT_CLASS(MaterialAbsorber, MaterialRT, "MaterialRT");
}

MaterialAbsorber::MaterialAbsorber()
{
    SO_NODE_CONSTRUCTOR(MaterialAbsorber);
    isBuiltIn = TRUE;
}

bool MaterialAbsorber::OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& rand, Ray& rayOut) const
{
    Q_UNUSED(rayIn)
    Q_UNUSED(dg)
    Q_UNUSED(rand)
    Q_UNUSED(rayOut)

    return false;
}
