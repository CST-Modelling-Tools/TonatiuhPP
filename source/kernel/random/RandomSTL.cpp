#include "RandomSTL.h"


RandomSTL::RandomSTL(ulong seed, ulong size):
    Random(size),
    m_generator(static_cast<std::mt19937_64::result_type>(seed)),
    m_distribution(0., 1.)
{

}

void RandomSTL::FillArray(std::vector<double>& array)
{
    for (ulong n = 0; n < array.size(); ++n)
        array[n] = m_distribution(m_generator);
}
