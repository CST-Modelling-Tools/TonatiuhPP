#include "RandomParallel.h"


RandomParallel::RandomParallel(RandomAbstract* rand, QMutex* mutex, ulong arraySize, QObject* parent)
    : QObject(parent), RandomAbstract(arraySize),
    m_pRand(rand),
    m_mutex(mutex)
{

}

void RandomParallel::FillArray(double* array, const ulong arraySize)
{
    m_mutex->lock();
    m_pRand->FillArray(array, arraySize);
    m_mutex->unlock();
}
