#pragma once

#include "kernel/air/AirAbstract.h"
#include <Inventor/fields/SoSFDouble.h>


class TONATIUH_KERNEL AirExponential: public AirAbstract
{
    SO_NODE_HEADER(AirExponential);

public:
    static void initClass();
    AirExponential();

    double transmission(double distance) const;
    SoSFDouble constant;

    NAME_ICON_FUNCTIONS("Exponential", ":/images/AirAbstract.png")
};
