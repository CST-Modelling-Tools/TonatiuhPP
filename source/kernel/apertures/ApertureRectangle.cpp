#include "ApertureRectangle.h"


SO_NODE_SOURCE(ApertureRectangle)

void ApertureRectangle::initClass()
{
     SO_NODE_INIT_CLASS(ApertureRectangle, Aperture, "Aperture");
}

ApertureRectangle::ApertureRectangle()
{
    SO_NODE_CONSTRUCTOR(ApertureRectangle);
    SO_NODE_ADD_FIELD( sizeX, (1.) );
    SO_NODE_ADD_FIELD( sizeY, (1.) );
}

bool ApertureRectangle::isInside(double x, double y) const
{
    return 2.*std::abs(x) <= sizeX.getValue() &&
           2.*std::abs(y) <= sizeY.getValue();
}
