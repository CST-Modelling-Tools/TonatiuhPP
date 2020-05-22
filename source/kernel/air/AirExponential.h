#pragma once

#include "kernel/air/Air.h"
#include <Inventor/fields/SoSFDouble.h>


class TONATIUH_KERNEL AirExponential: public Air
{
    SO_NODE_HEADER(AirExponential);

public:
    static void initClass();
    AirExponential();

    double transmission(double distance) const;
    SoSFDouble constant;

    NAME_ICON_FUNCTIONS("Exponential", ":/images/AirAbstract.png")
};
