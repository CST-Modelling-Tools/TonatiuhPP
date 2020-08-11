#include "AirTransmission.h"

SO_NODE_ABSTRACT_SOURCE(AirTransmission)


void AirTransmission::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(AirTransmission, SoNode, "Node");
}
