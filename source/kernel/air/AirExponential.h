#pragma once

#include "kernel/air/AirTransmission.h"


class TONATIUH_KERNEL AirExponential: public AirTransmission
{
    SO_NODE_HEADER(AirExponential);

public:
    static void initClass();
    AirExponential();

    double transmission(double distance) const;
    SoSFDouble constant;

    NAME_ICON_FUNCTIONS("Exponential", ":/air/AirAbstract.png")
};
