#pragma once

#include "libraries/TonatiuhLibraries.h"
#include <string>

namespace gf
{
    TONATIUH_LIBRARIES void SevereError(std::string errorMessage);
    TONATIUH_LIBRARIES void Warning(std::string warningMessage);
    TONATIUH_LIBRARIES bool IsOdd(int number);
    TONATIUH_LIBRARIES bool Quadratic(double A, double B, double C, double* t0, double* t1);
}
