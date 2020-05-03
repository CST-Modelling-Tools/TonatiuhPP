#pragma once

#include "kernel/random/RandomAbstract.h"
#include <random>


class TONATIUH_KERNEL RandomSTL: public RandomAbstract
{
public:
    RandomSTL(ulong seed, long size = 10'000'000);

    void FillArray(double* array, ulong size);

    NAME_ICON_FUNCTIONS("Mersenne-Twister(STL)", ":/RandomX.png")

private:
    std::mt19937_64 m_generator;
    std::uniform_real_distribution<double> m_distribution;
};
