#include "AirExponent.h"


SO_NODE_SOURCE(AirExponent)


void AirExponent::initClass()
{
    SO_NODE_INIT_CLASS(AirExponent, TTransmissivity, "TTransmissivity");
}

AirExponent::AirExponent()
{
    SO_NODE_CONSTRUCTOR(AirExponent);
    SO_NODE_ADD_FIELD(constant, (0.001));
    setName("Exponential");
}

double AirExponent::transmission( double distance) const
{
    return exp(-constant.getValue()*distance);
}
