#include "ApertureRing.h"


SO_NODE_SOURCE(ApertureRing)

void ApertureRing::initClass()
{
     SO_NODE_INIT_CLASS(ApertureRing, Aperture, "Aperture");
}

ApertureRing::ApertureRing()
{
    SO_NODE_CONSTRUCTOR(ApertureRing);
    SO_NODE_ADD_FIELD( sizeX, (1.) );
    SO_NODE_ADD_FIELD( sizeY, (1.) );
}

bool ApertureRing::isInside(double x, double y) const
{
    return 2.*std::abs(x) <= sizeX.getValue() &&
           2.*std::abs(y) <= sizeY.getValue();
}
