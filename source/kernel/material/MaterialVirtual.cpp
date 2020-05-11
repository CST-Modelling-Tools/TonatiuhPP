#include "MaterialVirtual.h"

#include <Inventor/sensors/SoFieldSensor.h>

#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/Ray.h"
#include "kernel/random/RandomAbstract.h"


SO_NODE_SOURCE(MaterialVirtual)

void MaterialVirtual::initClass()
{
     SO_NODE_INIT_CLASS(MaterialVirtual, MaterialAbstract, "MaterialAbstract");
}

MaterialVirtual::MaterialVirtual()
{
    SO_NODE_CONSTRUCTOR(MaterialVirtual);

    SO_NODE_ADD_FIELD( ambientColor, (0.2f, 0.2f, 0.2f) );
    SO_NODE_ADD_FIELD( diffuseColor, (0.8f, 0.8f, 0.8f) );
    SO_NODE_ADD_FIELD( specularColor, (0.f, 0.f, 0.f) );
    SO_NODE_ADD_FIELD( emissiveColor, (0.f, 0.f, 0.f) );
    SO_NODE_ADD_FIELD( shininess, (0.2f) );
    SO_NODE_ADD_FIELD( transparency, (0.f) );
}

bool MaterialVirtual::OutputRay(const Ray& rayIn, DifferentialGeometry* dg, RandomAbstract& /*rand*/, Ray* rayOut) const
{
    rayOut->origin = dg->point;
    rayOut->setDirection(rayIn.direction());
	return true;
}
