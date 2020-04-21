#include "gc.h"
#include "Point3D.h"
#include "Vector3D.h"
#include <iostream>

Point3D::Point3D(double dx, double dy, double dz)
    : x(dx), y(dy), z(dz)
{
}

Point3D::Point3D(const Vector3D& vector)
    : x(vector.x), y(vector.y), z(vector.z)
{
}

Point3D::~Point3D()
{
}

Point3D& Point3D::operator+=(const Vector3D& vector)
{
    x += vector.x;
    y += vector.y;
    z += vector.z;
    return *this;
}

Point3D Point3D::operator+(const Vector3D& vector) const
{
    return Point3D(x + vector.x, y + vector.y, z + vector.z);
}

Point3D& Point3D::operator-=(const Vector3D& vector)
{
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
    return *this;
}

Point3D Point3D::operator-(const Vector3D& vector) const
{
    return Point3D(x - vector.x, y - vector.y, z - vector.z);
}

Point3D& Point3D::operator*=(double scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Point3D Point3D::operator*(double scalar) const
{
    return Point3D(x * scalar, y * scalar, z * scalar);
}

Point3D& Point3D::operator/=(double scalar)
{
    double inv = 1.0 / scalar;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
}

Point3D Point3D::operator/(double scalar) const
{
    double inv = 1.0 / scalar;
    return Point3D(inv * x, inv * y, inv * z);
}

Vector3D Point3D::operator-(const Point3D& point) const
{
    return Vector3D(x - point.x, y - point.y, z - point.z);
}


bool Point3D::operator==(const Point3D& point) const
{
    if (this == &point) return true;
    else return (!(fabs(x - point.x) > gc::Epsilon) &&
             !(fabs(y - point.y) > gc::Epsilon) &&
             !(fabs(z - point.z) > gc::Epsilon) );
}

bool Point3D::operator!=(const Point3D& point) const
{
    if (this == &point) return false;
    else return ( (fabs(x - point.x) > gc::Epsilon) ||
              (fabs(y - point.y) > gc::Epsilon) ||
              (fabs(z - point.z) > gc::Epsilon) );
}

double Point3D::operator[](int i) const
{
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
}

double& Point3D::operator[](int i)
{
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
}

Point3D operator*(double scalar, const Point3D& point)
{
	return point * scalar;
}

std::ostream& operator<<(std::ostream& os, const Point3D& pA)
{
    os << pA.x << ", " << pA.y << ", " << pA.z;
    return os;
}

double Distance(const Point3D& pointA, const Point3D& pointB)
{
	return (pointA - pointB).length();
}

double DistanceSquared(const Point3D& pointA, const Point3D& pointB)
{
    return (pointA - pointB).lengthSquared();
}

