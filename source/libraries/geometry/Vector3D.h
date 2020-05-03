#pragma once

#include <iostream>

struct Point3D;
struct NormalVector;
#include "NormalVector.h"
#include "Point3D.h"
#include <cmath>

struct TONATIUH_LIBRARIES Vector3D
{
    Vector3D(double x = 0., double y = 0., double z = 0.):
        x(x), y(y), z(z)
    {
    }
    Vector3D(const NormalVector& norm):
        x(norm.x), y(norm.y), z(norm.z)
    {
    }

    explicit Vector3D(const Point3D& p):
        x(p.x), y(p.y), z(p.z)
    {
    }

    Vector3D& operator+=(const Vector3D& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3D& operator-=(const Vector3D& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    Vector3D& operator*=(double s)
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    Vector3D operator*(double s) const
    {
        return Vector3D(x*s, y*s, z*s);
    }

    Vector3D& operator/=(double s)
    {
        double inv = 1./s;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    Vector3D operator/(double s) const
    {
        double inv = 1./s;
        return Vector3D(x*inv, y*inv, z*inv);
    }

    Vector3D operator-() const
    {
        return Vector3D(-x, -y, -z);
    }

    bool operator==(const Vector3D& vector) const;
    bool operator!=(const Vector3D& vector) const;
    double operator[](int i) const
    {
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }

    double& operator[](int i)
    {
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }

    void zero();
    double lengthSquared() const
    {
        return x*x + y*y + z*z;
    }

    double length() const {
        return std::sqrt(x*x + y*y + z*z);
    }

    double norm() const {
        return std::sqrt(x*x + y*y + z*z);
    }

    double norm2() const {
        return x*x + y*y + z*z;
    }

    bool normalize()
    {
        double s = norm();
        if (s == 0.) return false;
        *this /= s;
        return true;
    }

    double x;
    double y;
    double z;
};

inline Vector3D operator+(Vector3D a, const Vector3D& b)
{
    //lhs take by value to let the compile to make the copy
    return a += b;
}
inline Vector3D operator-(Vector3D a, const Vector3D& b)
{
    //lhs take by value to let the compile to make the copy
    return a -= b;
}
inline Vector3D operator*(double s, const Vector3D& v)
{
    return Vector3D(s * v.x, s * v.y, s * v.z);
}

TONATIUH_LIBRARIES std::ostream& operator<<(std::ostream& os, const Vector3D& vector);

inline double dot(const Vector3D& a, const Vector3D& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline double dot(const Vector3D& v, const NormalVector& n)
{
    return v.x * n.x + v.y * n.y + v.z * n.z;
}

inline double dot(const NormalVector& n, const Vector3D& v)
{
    return n.x * v.x + n.y * v.y + n.z * v.z;
}

TONATIUH_LIBRARIES double AbsDotProduct(const Vector3D& vA, const Vector3D& vB);
TONATIUH_LIBRARIES double AbsDotProduct(const Vector3D& vA, const NormalVector& nB);
TONATIUH_LIBRARIES double AbsDotProduct(const NormalVector& nA, const Vector3D& vB);

inline Vector3D cross(const Vector3D& vA, const Vector3D& vB)
{
    return Vector3D(
        vA.y*vB.z - vA.z*vB.y,
        vA.z*vB.x - vA.x*vB.z,
        vA.x*vB.y - vA.y*vB.x
    );
}

inline Vector3D cross(const Vector3D& vA, const NormalVector& nB)
{
    return Vector3D( (vA.y * nB.z) - (vA.z * nB.y),
                     (vA.z * nB.x) - (vA.x * nB.z),
                     (vA.x * nB.y) - (vA.y * nB.x) );
}

inline Vector3D cross(const NormalVector& nA, const Vector3D& vB)
{
    return Vector3D( (nA.y * vB.z) - (nA.z * vB.y),
                     (nA.z * vB.x) - (nA.x * vB.z),
                     (nA.x * vB.y) - (nA.y * vB.x) );
}

inline Vector3D Normalize(const Vector3D& vA)
{
    if (vA.length() > 0.) return vA / vA.length();
    return vA;
}

TONATIUH_LIBRARIES bool SameHemisphere(const Vector3D& vA, const Vector3D& vB);
