#include "RandomParallel.h"


RandomParallel::RandomParallel(Random* rand, QMutex* mutex, ulong size, QObject* parent):
    QObject(parent),
    Random(size),
    m_rand(rand),
    m_mutex(mutex)
{

}

void RandomParallel::FillArray()
{
    m_mutex->lock();
    m_rand->FillArray();
    m_mutex->unlock();
}
