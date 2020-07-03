#include "Box2D.h"

#include "math/gcf.h"

const Box2D Box2D::UnitPositive(
    vec2d::Zero,
    vec2d::One
);
const Box2D Box2D::UnitCentered(
    -vec2d::One/2,
    vec2d::One/2
);


Box2D::Box2D():
    m_a(gcf::infinity, gcf::infinity),
    m_b(-gcf::infinity, -gcf::infinity)
{
}

void Box2D::setLimits(const vec2d& a, const vec2d& b)
{
   m_a = vec2d::min(a, b);
   m_b = vec2d::max(a, b);
}

double Box2D::area() const
{
    vec2d d = m_b - m_a;
    return d.x*d.y;
}

void Box2D::expandLimits(double delta)
{
    vec2d v(delta, delta);
    m_a -= v;
    m_b += v;
}

void Box2D::expand(const vec2d& p)
{
    m_a = vec2d::min(m_a, p);
    m_b = vec2d::max(m_b, p);
}

void Box2D::expand(const Box2D& b)
{
    m_a = vec2d::min(m_a, b.m_a);
    m_b = vec2d::max(m_b, b.m_b);
}
