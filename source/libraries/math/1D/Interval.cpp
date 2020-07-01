#include "Interval.h"
#include <cassert>
#include "math/gcf.h"


Interval::Interval(double a, double b)
{
    if (a <= b) {
        m_a = a;
        m_b = b;
    } else {
        m_a = b;
        m_b = a;
    }
}


double IntervalAngular::normalizeAngle(double alpha) const
{
    return gcf::normalizeAngle(alpha, m_a);
}
