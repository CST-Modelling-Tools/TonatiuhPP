#pragma once

#include "kernel/TonatiuhKernel.h"
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSubNode.h>


class TONATIUH_KERNEL TTransmissivity: public SoNode
{
    typedef SoNode inherited;

    SO_NODE_ABSTRACT_HEADER(TTransmissivity);

public:
    static void initClass();

    virtual double transmission(double /*distance*/) const {return 1.;}

    static const char* getClassName() {return "Air";}

protected:
    TTransmissivity() {}
    virtual ~TTransmissivity() {}
};
