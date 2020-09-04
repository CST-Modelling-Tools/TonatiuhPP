#include "MaterialVirtual.h"

#include "kernel/random/Random.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/math/3D/Ray.h"


SO_NODE_SOURCE(MaterialVirtual)

void MaterialVirtual::initClass()
{
     SO_NODE_INIT_CLASS(MaterialVirtual, MaterialRT, "MaterialRT");
}

MaterialVirtual::MaterialVirtual()
{
    SO_NODE_CONSTRUCTOR(MaterialVirtual);
    isBuiltIn = TRUE;
}

bool MaterialVirtual::OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& rand, Ray& rayOut) const
{
    Q_UNUSED(rand)

    rayOut.origin = dg.point;
    rayOut.setDirection(rayIn.direction());
	return true;
}
