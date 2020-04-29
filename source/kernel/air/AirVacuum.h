#pragma once

#include "AirAbstract.h"


class TONATIUH_KERNEL AirVacuum: public AirAbstract
{
    SO_NODE_HEADER(AirVacuum);

public:
    static void initClass();
    AirVacuum();

    double transmission(double /*distance*/) const {return 1.;}

    NAME_ICON_FUNCTIONS("Vacuum", ":/images/AirVacuum.png")
};
