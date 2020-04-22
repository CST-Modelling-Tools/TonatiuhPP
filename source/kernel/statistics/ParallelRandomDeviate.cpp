#include "RandomDeviate.h"
#include "ParallelRandomDeviate.h"

ParallelRandomDeviate::ParallelRandomDeviate(RandomDeviate* rand,  QMutex* mutex, unsigned long arraySize, QObject* parent)
    : QObject(parent),RandomDeviate(arraySize),
    m_pRand(rand),
    m_mutex(mutex)
{

}
ParallelRandomDeviate::~ParallelRandomDeviate()
{

}

void ParallelRandomDeviate::FillArray(double* array, const unsigned long arraySize)
{
    m_mutex->lock();
    m_pRand->FillArray(array, arraySize);
    m_mutex->unlock();
}
