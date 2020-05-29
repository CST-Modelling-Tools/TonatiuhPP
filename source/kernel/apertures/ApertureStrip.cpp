#include "ApertureStrip.h"


SO_NODE_SOURCE(ApertureStrip)

void ApertureStrip::initClass()
{
     SO_NODE_INIT_CLASS(ApertureStrip, Aperture, "Aperture");
}

ApertureStrip::ApertureStrip()
{
    SO_NODE_CONSTRUCTOR(ApertureStrip);
    SO_NODE_ADD_FIELD( sizeX, (1.) );
    SO_NODE_ADD_FIELD( sizeY, (1.) );
}

bool ApertureStrip::isInside(double x, double y) const
{
    return 2.*std::abs(x) <= sizeX.getValue() &&
           2.*std::abs(y) <= sizeY.getValue();
}
