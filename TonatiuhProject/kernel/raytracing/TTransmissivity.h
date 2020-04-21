#pragma once

#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSubNode.h>

#include "kernel/statistics/RandomDeviate.h"


class TTransmissivity: public SoNode
{
    typedef SoNode inherited;

    SO_NODE_ABSTRACT_HEADER(TTransmissivity);

public:
    static void initClass();

    virtual bool IsTransmitted(double distance, RandomDeviate& rand) const = 0;

protected:
    TTransmissivity() {}
    virtual ~TTransmissivity() {}
};
