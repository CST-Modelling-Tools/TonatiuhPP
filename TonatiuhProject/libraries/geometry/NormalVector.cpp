#include <cmath>
#include <float.h>

#include "NormalVector.h"
#include "Vector3D.h"

NormalVector::NormalVector(double dx, double dy, double dz)
    : x(dx), y(dy), z(dz)
{
}

NormalVector::NormalVector(const Vector3D& vector)
    : x(vector.x), y(vector.y), z(vector.z)
{
}

NormalVector::~NormalVector()
{
}

NormalVector& NormalVector::operator+=(const NormalVector& nV)
{
	x += nV.x;
    y += nV.y;
    z += nV.z;
    return *this;
}

NormalVector& NormalVector::operator-=(const NormalVector& nV)
{
    x -= nV.x;
    y -= nV.y;
    z -= nV.z;
    return *this;
}

NormalVector& NormalVector::operator*=(double scalar)
{
	x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

NormalVector NormalVector::operator*(double scalar) const
{
    return NormalVector(x * scalar, y * scalar, z * scalar);
}

NormalVector& NormalVector::operator/=(double scalar)
{
    double inv = 1.0 / scalar;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
}

NormalVector NormalVector::operator/(double scalar) const
{
    double inv = 1.0 / scalar;
    return NormalVector(x * inv, y * inv, z * inv);
}

NormalVector NormalVector::operator-() const
{
    return NormalVector(-x, -y, -z);
}

bool NormalVector::operator==(const NormalVector& nV) const
{
    if (this == &nV) return true;
    else return( (fabs(x - nV.x) < DBL_EPSILON) &&
             (fabs(y - nV.y) < DBL_EPSILON) &&
             (fabs(z - nV.z) < DBL_EPSILON) );
}

bool NormalVector::operator!=(const NormalVector& nV) const
{
    return !(*this == nV);
}

double NormalVector::operator[](int i) const
{
    if (i == 0) return x;
    else if (i == 1) return y;
    return z;
}

double& NormalVector::operator[](int i)
{
    if (i == 0) return x;
    else if (i == 1) return y;
    return z;
}

double NormalVector::lengthSquared() const
{
    return(x * x + y * y + z * z);
}

double NormalVector::length() const
{
    return sqrt(x * x + y * y + z * z);
}

NormalVector operator+(NormalVector lhs, const NormalVector& rhs)
{
	// Note that lhs is taken by value
	return lhs += rhs;
}

NormalVector operator-(NormalVector lhs, const NormalVector& rhs)
{
	// Note that lhs is taken by value
	return lhs -= rhs;
}

NormalVector operator*(double scalar, const NormalVector& nV)
{
    return NormalVector(scalar * nV.x, scalar * nV.y, scalar * nV.z);
}

std::ostream& operator<<(std::ostream& os, const NormalVector& nV)
{
	os << nV.x << ", " << nV.y << ", " << nV.z;
    return os;
}

double DotProduct(const NormalVector& nA, const NormalVector& nB)
{
    return nA.x * nB.x + nA.y * nB.y + nA.z * nB.z;
}

double AbsDotProduct(const NormalVector& nA, const NormalVector& nB)
{
    return fabs(DotProduct(nA, nB) );
}

NormalVector Normalize(const NormalVector& nV)
{
	return nV / nV.length();
}
