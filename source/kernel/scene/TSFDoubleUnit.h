#pragma once

#include "kernel/TonatiuhKernel.h"

#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoSubField.h>


class TONATIUH_KERNEL TSFDoubleUnit: public SoSField
{
    typedef SoSField inherited;

    SO_SFIELD_HEADER(TSFDoubleUnit, double, double)

public:
    static void initClass(void);

//    TSFDoubleUnit();

    enum Unit {
        Default = 0,
        Degree
    } unit;
};

