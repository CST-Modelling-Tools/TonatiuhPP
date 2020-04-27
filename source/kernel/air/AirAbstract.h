#pragma once

#include "kernel/TonatiuhKernel.h"
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSubNode.h>


class TONATIUH_KERNEL AirAbstract: public SoNode
{
    typedef SoNode inherited;

    SO_NODE_ABSTRACT_HEADER(AirAbstract);

public:
    static void initClass();

    virtual double transmission(double /*distance*/) const {return 1.;}

    static const char* getClassName() {return "Air";}

protected:
    AirAbstract() {}
    virtual ~AirAbstract() {}
};
