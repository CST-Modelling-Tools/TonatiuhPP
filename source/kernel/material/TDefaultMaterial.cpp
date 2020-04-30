#include <QString>

#include "TDefaultMaterial.h"

SO_NODE_SOURCE(TDefaultMaterial)


void TDefaultMaterial::initClass()
{
     SO_NODE_INIT_CLASS(TDefaultMaterial, MaterialAbstract, "Material");
}

TDefaultMaterial::TDefaultMaterial()
{
    SO_NODE_CONSTRUCTOR(TDefaultMaterial);
}

bool TDefaultMaterial::OutputRay(const Ray& /*incident*/, DifferentialGeometry* /*dg*/, RandomAbstract& /*rand*/, Ray* /*outputRay*/) const
{
   return false;
}




