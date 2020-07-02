#include "IntervalPeriodic.h"

#include "math/gcf.h"

double IntervalPeriodic::normalizeAngle(double alpha) const
{
    return gcf::normalizeAngle(alpha, m_a);
}
