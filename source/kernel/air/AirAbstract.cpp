#include "AirAbstract.h"

SO_NODE_ABSTRACT_SOURCE(AirAbstract)


void AirAbstract::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(AirAbstract, SoNode, "Node");
}
