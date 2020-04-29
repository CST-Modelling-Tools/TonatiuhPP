#pragma once

#include "kernel/random/RandomAbstract.h"
#include <random>


class RandomMersenneTwisterSTL: public RandomAbstract
{
public:
    RandomMersenneTwisterSTL(ulong seed, long size = 10'000'000);

    void FillArray(double* array, ulong size);

    NAME_ICON_FUNCTIONS("Mersenne-Twister(STL)", ":/RandomX.png")

private:
    std::mt19937_64 m_generator;
    std::uniform_real_distribution<double> m_distribution;
};



#include "kernel/random/RandomFactory.h"

class RandomMersenneTwisterSTLFactory:
    public QObject, public RandomFactoryT<RandomMersenneTwisterSTL>
{
    Q_OBJECT
    Q_INTERFACES(RandomFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.RandomFactory")
};
