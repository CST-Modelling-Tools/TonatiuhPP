#pragma once

#include "kernel/TonatiuhKernel.h"
#include "trt.h"
#include "TTransmissivity.h"


class TONATIUH_KERNEL TDefaultTransmissivity : public TTransmissivity
{
    SO_NODE_HEADER( TransmissivityDefualt );

public:
    static void initClass();
    TDefaultTransmissivity();

    bool transmission( double distance, RandomDeviate& rand ) const;

    trt::TONATIUH_REAL constant;

protected:
    virtual ~TDefaultTransmissivity();

};
