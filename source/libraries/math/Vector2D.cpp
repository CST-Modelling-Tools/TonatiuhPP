#include "Vector2D.h"

#include "gcf.h"

const Vector2D Vector2D::Zero(0., 0.);
const Vector2D Vector2D::UnitX(1., 0.);
const Vector2D Vector2D::UnitY(0., 1.);


bool Vector2D::operator==(const Vector2D& v) const
{
    if (this == &v) return true;

    return gcf::equals(x, v.x) && gcf::equals(y, v.y);
}

bool Vector2D::operator!=(const Vector2D& v) const
{
    return !(*this == v);
}

std::ostream& operator<<(std::ostream& os, const Vector2D& v)
{
    os << v.x << ", " << v.y;
    return os;
}
