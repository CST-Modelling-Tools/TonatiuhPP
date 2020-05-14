#pragma once

#include "libraries/TonatiuhLibraries.h"
#include <cmath>
#include <iostream>


struct TONATIUH_LIBRARIES Vector3D
{
    Vector3D(double x = 0., double y = 0., double z = 0.):
        x(x), y(y), z(z) {}

    // constants
    static const Vector3D Zero;
    static const Vector3D UnitX;
    static const Vector3D UnitY;
    static const Vector3D UnitZ;

    Vector3D operator+(const Vector3D& v) const
    {
        return Vector3D(x + v.x, y + v.y, z + v.z);
    }

    Vector3D& operator+=(const Vector3D& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3D operator-() const
    {
        return Vector3D(-x, -y, -z);
    }

    Vector3D operator-(const Vector3D& v) const
    {
        return Vector3D(x - v.x, y - v.y, z - v.z);
    }

    Vector3D& operator-=(const Vector3D& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3D operator*(double s) const
    {
        return Vector3D(x*s, y*s, z*s);
    }

    Vector3D& operator*=(double s)
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    Vector3D operator/(double s) const
    {
        s = 1./s;
        return Vector3D(x*s, y*s, z*s);
    }

    Vector3D& operator/=(double s)
    {
        s = 1./s;
        x *= s;
        y *= s;
        z *= s;
        return *this;
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


    double norm2() const {
        return x*x + y*y + z*z;
    }

    double norm() const {
        return std::sqrt(norm2());
    }

    Vector3D normalized() const
    {
        double s = norm2();
        if (s > 0.)
            return *this/sqrt(s);
        return *this;
    }

    bool normalize()
    {
        double s = norm2();
        if (s > 0.) {
            *this /= sqrt(s);
            return true;
        }
        return false;
    }

    Vector3D reflected(const Vector3D& n) const;

    double x;
    double y;
    double z;
};


inline Vector3D operator*(double s, const Vector3D& v)
{
    return Vector3D(s*v.x, s*v.y, s*v.z);
}

inline double dot(const Vector3D& a, const Vector3D& b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline Vector3D cross(const Vector3D& a, const Vector3D& b)
{
    return Vector3D(
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    );
}

inline Vector3D Vector3D::reflected(const Vector3D& n) const
{
    return *this - 2.*dot(*this, n)*n;
}

inline Vector3D min(const Vector3D& a, const Vector3D& b)
{
    return Vector3D(
        std::min(a.x, b.x),
        std::min(a.y, b.y),
        std::min(a.z, b.z)
    );
}

inline Vector3D max(const Vector3D& a, const Vector3D& b)
{
    return Vector3D(
        std::max(a.x, b.x),
        std::max(a.y, b.y),
        std::max(a.z, b.z)
    );
}

TONATIUH_LIBRARIES std::ostream& operator<<(std::ostream& os, const Vector3D& vector);
