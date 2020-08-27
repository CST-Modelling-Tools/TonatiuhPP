#include "MaterialTransparent.h"

#include "kernel/random/Random.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/math/3D/Ray.h"


SO_NODE_SOURCE(MaterialTransparent)

void MaterialTransparent::initClass()
{
     SO_NODE_INIT_CLASS(MaterialTransparent, MaterialRT, "MaterialRT");
}

MaterialTransparent::MaterialTransparent()
{
    SO_NODE_CONSTRUCTOR(MaterialTransparent);
}

bool MaterialTransparent::OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& rand, Ray& rayOut) const
{
    Q_UNUSED(rayIn)
    Q_UNUSED(dg)
    Q_UNUSED(rand)
    Q_UNUSED(rayOut)

    return false;
}
