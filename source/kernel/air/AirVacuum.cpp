#include "AirVacuum.h"

SO_NODE_SOURCE(AirVacuum)


void AirVacuum::initClass()
{
    SO_NODE_INIT_CLASS(AirVacuum, AirTransmission, "Air");
}

AirVacuum::AirVacuum()
{
    SO_NODE_CONSTRUCTOR(AirVacuum);
    setName("Vacuum");
}
