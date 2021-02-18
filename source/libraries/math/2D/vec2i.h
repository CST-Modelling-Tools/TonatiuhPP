#pragma once

#include "libraries/TonatiuhLibraries.h"


struct TONATIUH_LIBRARIES vec2i
{
    vec2i(int x = 0, int y = 0):
        x(x), y(y) {}

    vec2i operator+(const vec2i& v) const
    {
        return vec2i(x + v.x, y + v.y);
    }

    vec2i& operator+=(const vec2i& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    vec2i operator-() const
    {
        return vec2i(-x, -y);
    }

    vec2i operator-(const vec2i& v) const
    {
        return vec2i(x - v.x, y - v.y);
    }

    vec2i& operator-=(const vec2i& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    vec2i operator*(int s) const
    {
        return vec2i(x*s, y*s);
    }

    vec2i operator*(const vec2i& v) const
    {
        return vec2i(x*v.x, y*v.y);
    }

    vec2i& operator*=(int s)
    {
        x *= s;
        y *= s;
        return *this;
    }

    int operator[](int i) const
    {
        if (i == 0) return x;
        return y;
    }

    int& operator[](int i)
    {
        if (i == 0) return x;
        return y;
    }

    int x;
    int y;
};


inline vec2i operator*(int s, const vec2i& v)
{
    return vec2i(s*v.x, s*v.y);
}
