#include <QString>

#include "TDefaultMaterial.h"

SO_NODE_SOURCE(TDefaultMaterial);

void TDefaultMaterial::initClass()
{
     SO_NODE_INIT_CLASS( TDefaultMaterial, MaterialAbstract, "Material" );
}

TDefaultMaterial::TDefaultMaterial()
{
    SO_NODE_CONSTRUCTOR( TDefaultMaterial );
}

TDefaultMaterial::~TDefaultMaterial()
{
}


QString TDefaultMaterial::getIcon()
{
    return QLatin1String(":icons/TMaterial.png");
}

bool TDefaultMaterial::OutputRay( const Ray& /*incident*/, DifferentialGeometry* /*dg*/, RandomDeviate& /*rand*/, Ray* /*outputRay*/  ) const
{
   return false;
}




