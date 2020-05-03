#pragma once

#include "TrackerAbstract.h"
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/sensors/SoFieldSensor.h>


class TONATIUH_KERNEL TTrackerForAiming: public TrackerAbstract
{
    SO_NODEENGINE_ABSTRACT_HEADER(TTrackerForAiming);

public:
    static void initClass();

    SoSFBool isAimingAbsolute;

    void setAimingAbsolute(bool enable);

protected:
    TTrackerForAiming();
    ~TTrackerForAiming() {}
};
