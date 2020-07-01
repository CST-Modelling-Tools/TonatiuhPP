#pragma once

#include "libraries/TonatiuhLibraries.h"
#include <cmath>
#include <iostream>


struct TONATIUH_LIBRARIES vec2d
{
    vec2d(double x = 0., double y = 0):
        x(x), y(y) {}

    // constants
    static const vec2d Zero;
    static const vec2d One;
    static const vec2d UnitX;
    static const vec2d UnitY;

    vec2d operator+(const vec2d& v) const
    {
        return vec2d(x + v.x, y + v.y);
    }

    vec2d& operator+=(const vec2d& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    vec2d operator-() const
    {
        return vec2d(-x, -y);
    }

    vec2d operator-(const vec2d& v) const
    {
        return vec2d(x - v.x, y - v.y);
    }

    vec2d& operator-=(const vec2d& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    vec2d operator*(double s) const
    {
        return vec2d(x*s, y*s);
    }

    vec2d operator*(const vec2d& v) const
    {
        return vec2d(x*v.x, y*v.y);
    }

    vec2d& operator*=(double s)
    {
        x *= s;
        y *= s;
        return *this;
    }

    vec2d operator/(double s) const
    {
        s = 1./s;
        return vec2d(x*s, y*s);
    }

    vec2d operator/(const vec2d& v) const
    {
        return vec2d(x/v.x, y/v.y);
    }

    vec2d& operator/=(double s)
    {
        s = 1./s;
        x *= s;
        y *= s;
        return *this;
    }

    bool operator==(const vec2d& v) const;
    bool operator!=(const vec2d& v) const;
    bool operator<=(const vec2d& v) const;

    double operator[](int i) const
    {
        if (i == 0) return x;
        if (i == 1) return y;
    }

    double& operator[](int i)
    {
        if (i == 0) return x;
        return y;
    }

    double norm2() const {
        return x*x + y*y;
    }

    double norm() const {
        return std::sqrt(norm2());
    }

    vec2d normalized() const
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

    vec2d reflected(const vec2d& n) const;

    double x;
    double y;
};


inline vec2d operator*(double s, const vec2d& v)
{
    return vec2d(s*v.x, s*v.y);
}

inline double dot(const vec2d& a, const vec2d& b)
{
    return a.x*b.x + a.y*b.y;
}

inline double cross(const vec2d& a, const vec2d& b)
{
    return a.x*b.y - a.y*b.x;
}

inline vec2d min(const vec2d& a, const vec2d& b)
{
    return vec2d(
        std::min(a.x, b.x),
        std::min(a.y, b.y)
    );
}

inline vec2d max(const vec2d& a, const vec2d& b)
{
    return vec2d(
        std::max(a.x, b.x),
        std::max(a.y, b.y)
    );
}

TONATIUH_LIBRARIES std::ostream& operator<<(std::ostream& os, const vec2d& vector);
