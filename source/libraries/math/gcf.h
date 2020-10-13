#pragma once

#include "libraries/TonatiuhLibraries.h"

#include <cfloat>
#include <cmath>
#include <string>


// global constants and functions
namespace gcf
{
    const double pi = 3.1415926535897932385;
    const double TwoPi = 2.*pi;
    const double InvPi = 0.31830988618379067154;
    const double InvTwoPi = 0.15915494309189533577;
    const double degree = pi/180.0;
    const double infinity = HUGE_VAL;
    const double Epsilon = DBL_EPSILON;
    const double Ecliptic = 23.5*degree;

    template<class T>
    T pow2(T x) {return x*x;}

    template<class T>
    T clamp(T x, T a, T b)
    {
        if (x <= a) return a;
        if (x >= b) return b;
        return x;
    }

    template<class T>
    bool eqz(T x)
    {
        return std::abs(x) < std::numeric_limits<T>::epsilon();
    }

    template<class T>
    bool equals(T x, T y)
    {
        return std::abs(x - y) < std::numeric_limits<T>::epsilon();
    }

    template<class T, class U>
    T lerp(T a, T b, U u)
    {
        return (U(1) - u)*a + u*b;
    }

    TONATIUH_LIBRARIES double normalizeAngle(double phi, double phi0);

    TONATIUH_LIBRARIES void SevereError(std::string errorMessage);
    TONATIUH_LIBRARIES void Warning(std::string warningMessage);
    TONATIUH_LIBRARIES bool IsOdd(int number);
    TONATIUH_LIBRARIES bool solveQuadratic(double A, double B, double C, double* t0, double* t1);    
}
