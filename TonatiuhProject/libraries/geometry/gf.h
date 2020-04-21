#pragma once

#include <string>

namespace gf
{
void SevereError(std::string errorMessage);
void Warning(std::string warningMessage);
bool IsOdd(int number);
bool Quadratic(double A, double B, double C, double* t0, double* t1);
}
