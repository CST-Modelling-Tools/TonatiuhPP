#pragma once

#include "kernel/statistics/RandomDeviate.h"

#include <random>


class RandomMersenneTwisterSTL: public RandomDeviate
{

public:
    RandomMersenneTwisterSTL(ulong seed, long size = 10'000'000);

    void FillArray(double* array, ulong size);

    static const char* getClassName() {return "MersenneTwisterSTL";}

private:
    std::mt19937_64 m_generator;
    std::uniform_real_distribution<double> m_distribution;
};



#include "kernel/statistics/RandomDeviateFactory.h"

class RandomMersenneTwisterSTLFactory:
    public QObject, public RandomFactory<RandomMersenneTwisterSTL>
{
    Q_OBJECT
    Q_INTERFACES(RandomDeviateFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.RandomDeviateFactory")
};
