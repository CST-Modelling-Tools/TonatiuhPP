#pragma once

#include "libraries/TonatiuhLibraries.h"


class TONATIUH_LIBRARIES Interval
{
public:
    Interval(double a, double b);

    double min() const {return m_a;}
    double max() const {return m_b;}
    bool isInside(double x) const {return m_a <= x && x <= m_b;}
    double length() const {return m_b - m_a;}
    double mid() const {return (m_a + m_b)/2.;}

    double toNormalized(double x) const {return (x - m_a)/(m_b - m_a);}
    double fromNormalized(double u) const {return (1. - u)*m_a + u*m_b;}

protected:
    double m_a;
    double m_b;
};



class TONATIUH_LIBRARIES IntervalAngular: public Interval
{
public:
    IntervalAngular(double a, double b):
        Interval(a, b) {}

    double normalizeAngle(double alpha) const;
};
