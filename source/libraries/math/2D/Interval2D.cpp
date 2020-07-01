#include "Interval2D.h"


Interval2D::Interval2D(const vec2d& a, const vec2d& b)
{
   m_a = ::min(a, b);
   m_b = ::max(a, b);
}
