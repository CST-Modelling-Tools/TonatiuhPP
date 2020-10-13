#include <cmath>
#include <iostream>
#include <stdlib.h>

#include "gcf.h"


void gcf::SevereError(std::string errorMessage)
{
    std::cerr << errorMessage << std::endl;
    exit(-1);
}

void gcf::Warning(std::string warningMessage)
{
    std::cerr << warningMessage << std::endl;
}

bool gcf::IsOdd(int number)
{
    bool answer = number & 1;
    return answer;
}

bool gcf::solveQuadratic(double a, double b, double c, double* t0, double* t1)
{
    double d = b*b - 4.*a*c;
    if (d < 0.) return false;

    double q = -0.5;
    if (b > 0.)
        q *= b + sqrt(d);
    else
        q *= b - sqrt(d);

    *t0 = q/a;
    *t1 = c/q;
    if (*t0 > *t1) std::swap(*t0, *t1);
    return true;
}

// normalize angle phi to the range [phi0, phi0 + 2pi)
double gcf::normalizeAngle(double phi, double phi0)
{
    return phi - TwoPi*floor((phi - phi0)/TwoPi);
}
