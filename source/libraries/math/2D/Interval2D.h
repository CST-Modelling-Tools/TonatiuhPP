#pragma once

#include "libraries/math/2D/vec2d.h"


class TONATIUH_LIBRARIES Interval2D
{
public:
    Interval2D(const vec2d& a, const vec2d& b);
    Interval2D(): Interval2D(vec2d::Zero, vec2d::One) {}

    vec2d min() const {return m_a;}
    vec2d max() const {return m_b;}
    vec2d size() const {return m_b - m_a;}
    vec2d mid() const {return (m_a + m_b)/2.;}

    bool isInside(const vec2d& v) const {return m_a <= v && v <= m_b;}

    vec2d toNormalized(const vec2d& v) const {return (v - m_a)/(m_b - m_a);}
    vec2d fromNormalized(const vec2d& vN) const {return (vec2d::One - vN)*m_a + vN*m_b;}

protected:
    vec2d m_a;
    vec2d m_b;
};


