#pragma once

#include "libraries/math/2D/Matrix2D.h"
#include "libraries/math/2D/vec2d.h"


template<class T>
T interpolateLinear(const Matrix2D<T>& matrix, const vec2d& v)
{
    int nMax = matrix.rows() - 1;
    int n = std::floor(v.x);
    double x;
    if (n < 0) {
        n = 0;
        x = 0.;
    } else if (n >= nMax) {
        n = nMax - 1;
        x = 1.;
    } else
        x = v.x - n;

    int mMax = matrix.cols() - 1;
    int m = std::floor(v.y);
    double y;
    if (m < 0) {
        m = 0;
        y = 0.;
    } else if (m >= mMax) {
        m = mMax - 1;
        y = 1.;
    } else
        y = v.y - m;

    double a = (1. - x)*(1. - y);
    double b = x*(1. - y);
    double c = (1. - x)*y;
    double d = x*y;
    return a*matrix(n, m) + b*matrix(n + 1, m) + c*matrix(n, m + 1) + d*matrix(n + 1, m + 1);
}
