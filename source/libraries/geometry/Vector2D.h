#pragma once

#include "libraries/TonatiuhLibraries.h"
#include <cmath>
#include <iostream>


struct TONATIUH_LIBRARIES Vector2D
{
    Vector2D(double x = 0., double y = 0):
        x(x), y(y) {}

    // constants
    static const Vector2D Zero;
    static const Vector2D UnitX;
    static const Vector2D UnitY;

    Vector2D operator+(const Vector2D& v) const
    {
        return Vector2D(x + v.x, y + v.y);
    }

    Vector2D& operator+=(const Vector2D& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2D operator-() const
    {
        return Vector2D(-x, -y);
    }

    Vector2D operator-(const Vector2D& v) const
    {
        return Vector2D(x - v.x, y - v.y);
    }

    Vector2D& operator-=(const Vector2D& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vector2D operator*(double s) const
    {
        return Vector2D(x*s, y*s);
    }

    Vector2D& operator*=(double s)
    {
        x *= s;
        y *= s;
        return *this;
    }

    Vector2D operator/(double s) const
    {
        s = 1./s;
        return Vector2D(x*s, y*s);
    }

    Vector2D& operator/=(double s)
    {
        s = 1./s;
        x *= s;
        y *= s;
        return *this;
    }

    bool operator==(const Vector2D& vector) const;
    bool operator!=(const Vector2D& vector) const;

    double operator[](int i) const
    {
        if (i == 0) return x;
        if (i == 1) return y;
    }

    double& operator[](int i)
    {
        if (i == 0) return x;
        if (i == 1) return y;
    }

    double norm2() const {
        return x*x + y*y;
    }

    double norm() const {
        return std::sqrt(norm2());
    }

    Vector2D normalized() const
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

    Vector2D reflected(const Vector2D& n) const;

    double x;
    double y;
};


inline Vector2D operator*(double s, const Vector2D& v)
{
    return Vector2D(s*v.x, s*v.y);
}

inline double dot(const Vector2D& a, const Vector2D& b)
{
    return a.x*b.x + a.y*b.y;
}

inline Vector2D min(const Vector2D& a, const Vector2D& b)
{
    return Vector2D(
        std::min(a.x, b.x),
        std::min(a.y, b.y)
    );
}

inline Vector2D max(const Vector2D& a, const Vector2D& b)
{
    return Vector2D(
        std::max(a.x, b.x),
        std::max(a.y, b.y)
    );
}

TONATIUH_LIBRARIES std::ostream& operator<<(std::ostream& os, const Vector2D& vector);
