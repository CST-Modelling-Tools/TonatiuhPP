#pragma once

#include "kernel/statistics/RandomDeviate.h"

const double LongIntegerToDouble = 1.0 / 4294967296.0;

class RandomMersenneTwisterSTL: public RandomDeviate
{

public:
    RandomMersenneTwisterSTL(unsigned long seedValue = 5489UL, long int randomNumberArraySize = 10000000);
    RandomMersenneTwisterSTL(const unsigned long* seedArray, int seedArraySize, long int randomNumberArraySize = 10000000);
    virtual ~RandomMersenneTwisterSTL() {}
    void FillArray(double* array, const unsigned long arraySize);
    unsigned long RandomUInt();

    static const char* getClassName() {return "MersenneTwister";}

private:
    enum { N = 624, M = 397 };

    unsigned long m_state[N];
    int m_p;
    bool m_init;

    void Seed(unsigned long seedValue);
    void Seed(const unsigned long* seedArray, int arraySize);
    unsigned long int RandomInteger();
    double Random01();
    unsigned long Twiddle(unsigned long u, unsigned long v);
    void GenerateNewState();

    RandomMersenneTwisterSTL(const RandomMersenneTwisterSTL&);
    void operator=(const RandomMersenneTwisterSTL&);
};

inline RandomMersenneTwisterSTL::RandomMersenneTwisterSTL(unsigned long seedValue, long int randomNumberArraySize)
    : RandomDeviate(randomNumberArraySize), m_p(0)
{
    Seed(seedValue);
    m_init = true;
}

inline RandomMersenneTwisterSTL::RandomMersenneTwisterSTL(const unsigned long* seedArray, int seedArraySize, long int randomNumberArraySize)
    : RandomDeviate(randomNumberArraySize), m_p(0)
{
    Seed(seedArray, seedArraySize);
    m_init = true;
}


inline void RandomMersenneTwisterSTL::FillArray(double* array, const unsigned long arraySize)
{
    for (unsigned int i = 0; i < arraySize; i++) array[i] = Random01();
}

inline unsigned long RandomMersenneTwisterSTL::Twiddle(unsigned long u, unsigned long v)
{
    return ( ( (u & 0x80000000UL) | (v & 0x7FFFFFFFUL) ) >> 1)
           ^ ( (v & 1UL) ? 0x9908B0DFUL : 0x0UL);
}

inline unsigned long int RandomMersenneTwisterSTL::RandomInteger()
{
    if (m_p == N) GenerateNewState(); // new state vector needed
    unsigned long x = m_state[m_p++];
    x ^= (x >> 11);
    x ^= (x << 7) & 0x9D2C5680UL;
    x ^= (x << 15) & 0xEFC60000UL;
    return x ^ (x >> 18);
}

inline double RandomMersenneTwisterSTL::Random01()
{
    return (static_cast<double>(RandomInteger() ) + 0.5) * LongIntegerToDouble;   // divided by 2^32
}



#include "kernel/statistics/RandomDeviateFactory.h"

class RandomMersenneTwisterSTLFactory:
    public QObject, public RandomFactory<RandomMersenneTwisterSTL>
{
    Q_OBJECT
    Q_INTERFACES(RandomDeviateFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.RandomDeviateFactory")
};
