#include "vec3d.h"

#include "math/gcf.h"

const vec3d vec3d::Zero(0., 0., 0.);
const vec3d vec3d::One(1., 1., 1.);
const vec3d vec3d::UnitX(1., 0., 0.);
const vec3d vec3d::UnitY(0., 1., 0.);
const vec3d vec3d::UnitZ(0., 0., 1.);


bool vec3d::operator==(const vec3d& v) const
{
    return gcf::equals(x, v.x) &&
           gcf::equals(y, v.y) &&
           gcf::equals(z, v.z);
}

bool vec3d::operator!=(const vec3d& v) const
{
    return !(*this == v);
}

bool vec3d::operator<=(const vec3d& v) const
{
    return x <= v.x && y <= v.y && z < v.z;
}

int vec3d::maxDimension() const
{
    if (x > y && x > z)
        return 0;
    else if (y > z)
        return 1;
    return 2;
}

std::ostream& operator<<(std::ostream& os, const vec3d& v)
{
    os << v.x << ", " << v.y << ", " << v.z;
    return os;
}
