#pragma once

#include "libraries/math/1D/Interval.h"


class TONATIUH_LIBRARIES IntervalPeriodic: public Interval
{
public:
    IntervalPeriodic(double a, double b):
        Interval(a, b) {}

    double normalizeAngle(double alpha) const;
};
