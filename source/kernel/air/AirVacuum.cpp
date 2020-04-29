#include "AirVacuum.h"

SO_NODE_SOURCE(AirVacuum)


void AirVacuum::initClass()
{
    SO_NODE_INIT_CLASS(AirVacuum, AirAbstract, "AirAbstract");
}

AirVacuum::AirVacuum()
{
    SO_NODE_CONSTRUCTOR(AirVacuum);
}
