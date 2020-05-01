#pragma once

#include "TTracker.h"
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/sensors/SoFieldSensor.h>


class TONATIUH_KERNEL TTrackerForAiming:  public TTracker
{
    SO_NODEENGINE_ABSTRACT_HEADER(TTrackerForAiming);

public:
    static void initClass();

    enum AimingPointType {
        Absolute = 0,
        Relative = 1
    };

    SoSFEnum typeOfAimingPoint;

    static void updateTypeOfAimingPoint(void* data, SoSensor*);
    virtual void SwitchAimingPointType() = 0;
    void SetAimingPointRelativity(bool relative);

protected:
    TTrackerForAiming();
    ~TTrackerForAiming();
};