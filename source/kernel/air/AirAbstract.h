#pragma once

#include "kernel/scene/TAbstract.h"
#include <Inventor/nodes/SoSubNode.h>


class TONATIUH_KERNEL AirAbstract: public SoNode
{
    SO_NODE_ABSTRACT_HEADER(AirAbstract);

public:
    static void initClass();

    virtual double transmission(double /*distance*/) const = 0;

    NAME_ICON_FUNCTIONS("X", ":/AirX.png")

protected:
    AirAbstract() {}
    ~AirAbstract() {}
};
