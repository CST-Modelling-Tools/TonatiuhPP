#include "MaterialFresnel.h"

#include "libraries/math/3D/Ray.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "kernel/random/Random.h"


SO_NODE_SOURCE(MaterialFresnel)

void MaterialFresnel::initClass()
{
     SO_NODE_INIT_CLASS(MaterialFresnel, MaterialRT, "MaterialRT");
}

MaterialFresnel::MaterialFresnel()
{
    SO_NODE_CONSTRUCTOR(MaterialFresnel);
    isBuiltIn = TRUE;
}

bool MaterialFresnel::OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& rand, Ray& rayOut) const
{
    Q_UNUSED(rayIn)
    Q_UNUSED(dg)
    Q_UNUSED(rand)
    Q_UNUSED(rayOut)

    return false;
}
