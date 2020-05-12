#pragma once

#include "TrackerAbstract.h"
#include "kernel/TonatiuhTypes.h"


class TONATIUH_KERNEL TTrackerForAiming: public TrackerAbstract
{
    SO_NODE_ABSTRACT_HEADER(TTrackerForAiming);

public:
    static void initClass();

    SoSFBool isAimingAbsolute;

    void setAimingAbsolute(bool enable);

protected:
    TTrackerForAiming();
    ~TTrackerForAiming() {}
};
