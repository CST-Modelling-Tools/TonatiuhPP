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

bool gf::Quadratic(double A, double B, double C, double* t0, double* t1)
{
	// Find discriminant
    double discrim = B * B - 4.0 * A * C;
	if (discrim < 0.) return false;

	// Compute quadratic root values
	double q = -0.5;
    if (B < 0) q *= B - sqrt(discrim);
    else q *= B + sqrt(discrim);
	*t0 = q / A;
	*t1 = C / q;
    if (*t0 > *t1) std::swap(*t0, *t1);
	return true;
}
