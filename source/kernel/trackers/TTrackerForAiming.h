#pragma once

#include "TrackerAbstract.h"
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/sensors/SoFieldSensor.h>


class TONATIUH_KERNEL TTrackerForAiming: public TrackerAbstract
{
    SO_NODEENGINE_ABSTRACT_HEADER(TTrackerForAiming);

public:
    static void initClass();

    enum AimingPointType {
        Absolute = 0,
        Relative = 1
    };

    SoSFEnum typeOfAimingPoint;

    void SetAimingPointRelativity(bool relative);

protected:
    TTrackerForAiming();
    ~TTrackerForAiming() {}
};
