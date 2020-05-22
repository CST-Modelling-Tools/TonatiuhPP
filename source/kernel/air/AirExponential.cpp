#include "AirExponential.h"


SO_NODE_SOURCE(AirExponential)


void AirExponential::initClass()
{
    SO_NODE_INIT_CLASS(AirExponential, Air, "AirAbstract");
}

AirExponential::AirExponential()
{
    SO_NODE_CONSTRUCTOR(AirExponential);
    setName("Exponential");
    SO_NODE_ADD_FIELD(constant, (0.001));
}

double AirExponential::transmission(double distance) const
{
    return exp(-constant.getValue()*distance);
}
