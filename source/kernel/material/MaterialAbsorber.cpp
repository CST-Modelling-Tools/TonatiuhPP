#include "MaterialAbsorber.h"

#include <Inventor/sensors/SoFieldSensor.h>

#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/Ray.h"
#include "kernel/random/Random.h"


SO_NODE_SOURCE(MaterialAbsorber)

void MaterialAbsorber::initClass()
{
     SO_NODE_INIT_CLASS(MaterialAbsorber, MaterialRT, "MaterialRT");
}

MaterialAbsorber::MaterialAbsorber()
{
    SO_NODE_CONSTRUCTOR(MaterialAbsorber);

    SO_NODE_ADD_FIELD( ambientColor, (0.2f, 0.2f, 0.2f) );
    SO_NODE_ADD_FIELD( diffuseColor, (0.8f, 0.8f, 0.8f) );
    SO_NODE_ADD_FIELD( specularColor, (0.f, 0.f, 0.f) );
    SO_NODE_ADD_FIELD( emissiveColor, (0.f, 0.f, 0.f) );
    SO_NODE_ADD_FIELD( shininess, (0.2f) );
    SO_NODE_ADD_FIELD( transparency, (0.f) );
}

bool MaterialAbsorber::OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& /*rand*/, Ray& rayOut) const
{
    return false;
}
