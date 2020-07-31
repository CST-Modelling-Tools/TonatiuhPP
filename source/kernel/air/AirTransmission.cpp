#include "AirTransmission.h"

SO_NODE_SOURCE(AirTransmission)


void AirTransmission::initClass()
{
    SO_NODE_INIT_CLASS(AirTransmission, SoNode, "Node");
}

AirTransmission::AirTransmission()
{
    SO_NODE_CONSTRUCTOR(AirTransmission);
}
