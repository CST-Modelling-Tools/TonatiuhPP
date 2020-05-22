#include "RandomSTL.h"


RandomSTL::RandomSTL(ulong seed, ulong size):
    Random(size),
    m_generator(static_cast<std::mt19937_64::result_type>(seed)),
    m_distribution(0., 1.)
{

}

void RandomSTL::FillArray()
{
    for (ulong n = 0; n < m_array.size(); ++n)
        m_array[n] = m_distribution(m_generator);
}
