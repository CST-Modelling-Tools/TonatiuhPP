#pragma once

#include "kernel/TonatiuhKernel.h"
#include "kernel/raytracing/trt.h"
#include "AirAbstract.h"


class TONATIUH_KERNEL TDefaultTransmissivity: public AirAbstract
{
    SO_NODE_HEADER(TransmissivityDefualt);

public:
    static void initClass();
    TDefaultTransmissivity();

    double transmission(double distance) const;

    trt::TONATIUH_REAL constant;

protected:
    virtual ~TDefaultTransmissivity();
};
