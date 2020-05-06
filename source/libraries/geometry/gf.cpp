#include <cmath>
#include <iostream>
#include <stdlib.h>

#include "gc.h"
#include "gf.h"


void gf::SevereError(std::string errorMessage)
{
    std::cerr << errorMessage << std::endl;
    exit(-1);
}

void gf::Warning(std::string warningMessage)
{
    std::cerr << warningMessage << std::endl;
}

bool gf::IsOdd(int number)
{
    bool answer = number & 1;
    return answer;
}

bool gf::solveQuadratic(double a, double b, double c, double* t0, double* t1)
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
