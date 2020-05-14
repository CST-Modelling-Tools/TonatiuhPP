#include <cmath>
#include <float.h>
#include <iostream>

#include "NormalVector.h"
#include "Point3D.h"
#include "Vector3D.h"



bool Vector3D::operator==(const Vector3D& v) const
{
    if (this == &v) return true;

    return fabs(x - v.x) < DBL_EPSILON &&
           fabs(y - v.y) < DBL_EPSILON &&
           fabs(z - v.z) < DBL_EPSILON;
}

bool Vector3D::operator!=(const Vector3D& v) const
{
    return !(*this == v);
}

void Vector3D::zero()
{
    x = 0.;
    y = 0.;
    z = 0.;
}

std::ostream& operator<<(std::ostream& os, const Vector3D& v)
{
    os << v.x << ", " << v.y << ", " << v.z;
    return os;
}

double AbsDotProduct(const Vector3D& vA, const Vector3D& vB)
{
    return fabs(dot(vA, vB) );
}

bool SameHemisphere(const Vector3D& vA, const Vector3D& vB)
{
    return (vA.z * vB.z > 0.0);
}
