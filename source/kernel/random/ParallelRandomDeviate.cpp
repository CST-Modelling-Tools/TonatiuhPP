#include "ParallelRandomDeviate.h"


ParallelRandomDeviate::ParallelRandomDeviate(RandomDeviate* rand, QMutex* mutex, ulong arraySize, QObject* parent)
    : QObject(parent), RandomDeviate(arraySize),
    m_pRand(rand),
    m_mutex(mutex)
{

}

void ParallelRandomDeviate::FillArray(double* array, const ulong arraySize)
{
    m_mutex->lock();
    m_pRand->FillArray(array, arraySize);
    m_mutex->unlock();
}
