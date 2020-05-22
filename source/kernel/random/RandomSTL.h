#pragma once

#include "kernel/random/Random.h"
#include <random>


class TONATIUH_KERNEL RandomSTL: public Random
{
public:
    RandomSTL(ulong seed, ulong size = 10'000'000);

    void FillArray(std::vector<double>& array);

    NAME_ICON_FUNCTIONS("Mersenne-Twister(STL)", ":/RandomX.png")

protected:
    std::mt19937_64 m_generator;
    std::uniform_real_distribution<double> m_distribution;
};
