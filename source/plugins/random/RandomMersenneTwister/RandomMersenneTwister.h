#pragma once

#include "kernel/random/RandomDeviate.h"

const double LongIntegerToDouble = 1.0 / 4294967296.0;

class RandomMersenneTwister: public RandomDeviate
{

public:
    RandomMersenneTwister(ulong seedValue = 5489UL, long randomNumberArraySize = 10'000'000);
    RandomMersenneTwister(const ulong* seedArray, int seedArraySize, long randomNumberArraySize = 10'000'000);
    virtual ~RandomMersenneTwister() {}

    void FillArray(double* array, const ulong arraySize);
    ulong RandomUInt();

    static const char* getClassName() {return "MersenneTwister";}

private:
    enum { N = 624, M = 397 };

    ulong m_state[N];
    int m_p;
    bool m_init;

    void Seed(ulong seedValue);
    void Seed(const ulong* seedArray, int arraySize);
    ulong RandomInteger();
    double Random01();
    ulong Twiddle(ulong u, ulong v);
    void GenerateNewState();

    RandomMersenneTwister(const RandomMersenneTwister&);
    void operator=(const RandomMersenneTwister&);
};

inline RandomMersenneTwister::RandomMersenneTwister(unsigned long seedValue, long int randomNumberArraySize)
    : RandomDeviate(randomNumberArraySize), m_p(0)
{
    Seed(seedValue);
    m_init = true;
}

inline RandomMersenneTwister::RandomMersenneTwister(const ulong* seedArray, int seedArraySize, long int randomNumberArraySize)
    : RandomDeviate(randomNumberArraySize), m_p(0)
{
    Seed(seedArray, seedArraySize);
    m_init = true;
}

inline void RandomMersenneTwister::FillArray(double* array, const ulong arraySize)
{
    for (uint i = 0; i < arraySize; i++) array[i] = Random01();
}

inline ulong RandomMersenneTwister::Twiddle(ulong u, ulong v)
{
    return ( ( (u & 0x80000000UL) | (v & 0x7FFFFFFFUL) ) >> 1)
           ^ ( (v & 1UL) ? 0x9908B0DFUL : 0x0UL);
}

inline ulong RandomMersenneTwister::RandomInteger()
{
    if (m_p == N) GenerateNewState(); // new state vector needed
    ulong x = m_state[m_p++];
    x ^= (x >> 11);
    x ^= (x << 7) & 0x9D2C5680UL;
    x ^= (x << 15) & 0xEFC60000UL;
    return x ^ (x >> 18);
}

inline double RandomMersenneTwister::Random01()
{
    return (static_cast<double>(RandomInteger() ) + 0.5) * LongIntegerToDouble;   // divided by 2^32
}



#include "kernel/random/RandomDeviateFactory.h"

class RandomMersenneTwisterFactory:
    public QObject, public RandomFactory<RandomMersenneTwister>
{
    Q_OBJECT
    Q_INTERFACES(RandomDeviateFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.RandomDeviateFactory")
};
