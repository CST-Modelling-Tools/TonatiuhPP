#include <cmath>
#include <float.h>
#include <iostream>

#include "NormalVector.h"
#include "Point3D.h"
#include "Vector3D.h"



bool Vector3D::operator==(const Vector3D& vector) const
{
    if (this == &vector) return true;
    else return( (fabs(x - vector.x) < DBL_EPSILON) &&
             (fabs(y - vector.y) < DBL_EPSILON) &&
             (fabs(z - vector.z) < DBL_EPSILON) );
}

bool Vector3D::operator!=(const Vector3D& vector) const
{
    return !(*this == vector);
}


void Vector3D::zero()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}


std::ostream& operator<<(std::ostream& os, const Vector3D& vector)
{
    os << vector.x << ", " << vector.y << ", " << vector.z;
    return os;
}


double AbsDotProduct(const Vector3D& vA, const Vector3D& vB)
{
    return fabs(DotProduct(vA, vB) );
}

double AbsDotProduct(const Vector3D& vA, const NormalVector& nB)
{
    return fabs(DotProduct(vA, nB) );
}

double AbsDotProduct(const NormalVector& nA, const Vector3D& vB)
{
    return fabs(DotProduct(nA, vB) );
}





bool SameHemisphere(const Vector3D& vA, const Vector3D& vB)
{
    return (vA.z * vB.z > 0.0);
}
