#pragma once

#include "libraries/math/2D/vec2d.h"


class TONATIUH_LIBRARIES Box2D
{
public:
    Box2D(const vec2d& a, const vec2d& b);
    Box2D(): Box2D(vec2d::Zero, vec2d::One) {} // use empty?

    vec2d min() const {return m_a;}
    vec2d max() const {return m_b;}
    vec2d size() const {return m_b - m_a;}
    vec2d center() const {return (m_a + m_b)/2.;}
    double area() const;

    void expandLimits(double delta);
    void expand(const vec2d& p);
    void expand(const Box2D& b);
    void operator<<(const vec2d& p) {expand(p);}
    void operator<<(const Box2D& b) {expand(b);}

    bool isInside(const vec2d& p) const {return m_a <= p && p <= m_b;}

    vec2d toNormalized(const vec2d& v) const {return (v - m_a)/(m_b - m_a);}
    vec2d fromNormalized(const vec2d& vN) const {return (vec2d::One - vN)*m_a + vN*m_b;}

    static const Box2D UnitPositive;
    static const Box2D UnitCentered;

protected:
    vec2d m_a;
    vec2d m_b;
};


