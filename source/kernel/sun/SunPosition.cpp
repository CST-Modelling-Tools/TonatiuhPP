#include "SunPosition.h"

SO_NODE_SOURCE(SunPosition)
#include "libraries/math/gcf.h"

void SunPosition::initClass()
{
    SO_NODE_INIT_CLASS(SunPosition, SoNode, "Node");
}

SunPosition::SunPosition()
{
    SO_NODE_CONSTRUCTOR(SunPosition);
    SO_NODE_ADD_FIELD( azimuth, (0.) );
    SO_NODE_ADD_FIELD( elevation, (90.*gcf::degree) );
    SO_NODE_ADD_FIELD( irradiance, (1000.) );
}

vec3d SunPosition::getSunVector()
{
    double az = azimuth.getValue();
    double el = elevation.getValue();

    return vec3d(
        sin(az)*cos(el),
        cos(az)*cos(el),
        sin(el)
    );
}
