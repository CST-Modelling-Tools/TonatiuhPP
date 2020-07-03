#pragma once

#include "libraries/TonatiuhLibraries.h"


class TONATIUH_LIBRARIES Interval
{
public:
    Interval();
    Interval(double a, double b) {setLimits(a, b);}
    void setLimits(double a, double b);
    bool isValid() const {return m_a <= m_b;}

    double min() const {return m_a;}
    double max() const {return m_b;}
    double size() const {return m_b - m_a;}
    double mid() const {return (m_a + m_b)/2.;}

    void expandLimits(double delta);
    void expand(double x);
    void expand(const Interval& b);
    void operator<<(double p) {expand(p);}
    void operator<<(const Interval& b) {expand(b);}

    bool isInside(double x) const {return m_a <= x && x <= m_b;}
    bool intersect(const Interval& b) const {return m_a <= b.m_b && b.m_a <= m_b;}

    double toNormalized(double x) const {return (x - m_a)/(m_b - m_a);}
    double fromNormalized(double u) const {return (1. - u)*m_a + u*m_b;}

    static const Interval UnitPositive;
    static const Interval UnitCentered;

protected:
    double m_a;
    double m_b;
};
