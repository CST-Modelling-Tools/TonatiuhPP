#include "RandomMersenneTwisterSTL.h"


RandomMersenneTwisterSTL::RandomMersenneTwisterSTL(ulong seed, long size):
    RandomDeviate(size),
    m_generator{static_cast<std::mt19937_64::result_type>(seed)},
    m_distribution(0., 1.)
{

}

void RandomMersenneTwisterSTL::FillArray(double* array, ulong size)
{
    for (ulong i = 0; i < size; i++)
        array[i] = m_distribution(m_generator);
}
