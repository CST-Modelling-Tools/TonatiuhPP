#include "Interval.h"

#include "math/gcf.h"

const Interval Interval::UnitPositive(0., 1.);
const Interval Interval::UnitCentered(-0.5, 0.5);


Interval::Interval():
    m_a(gcf::infinity),
    m_b(-gcf::infinity)
{
}

void Interval::setLimits(double a, double b)
{
    if (a <= b) {
        m_a = a;
        m_b = b;
    } else {
        m_a = b;
        m_b = a;
    }
}

void Interval::expandLimits(double delta)
{
    if (delta < 0.) return;
    m_a -= delta;
    m_b += delta;
}

void Interval::expand(double x)
{
    m_a = std::min(m_a, x);
    m_b = std::max(m_b, x);
}

void Interval::expand(const Interval& b)
{
    m_a = std::min(m_a, b.m_a);
    m_b = std::max(m_b, b.m_b);
}

