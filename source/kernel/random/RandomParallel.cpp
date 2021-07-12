#include "RandomParallel.h"


RandomParallel::RandomParallel(Random* rand, QMutex* mutex, ulong size):
    Random(size),
    m_rand(rand),
    m_mutex(mutex)
{

}

void RandomParallel::FillArray(std::vector<double>& array)
{
    m_mutex->lock();
    m_rand->FillArray(array);
    m_mutex->unlock();
}
