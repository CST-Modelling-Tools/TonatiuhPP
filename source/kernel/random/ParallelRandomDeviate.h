#pragma once

#include "kernel/TonatiuhKernel.h"
#include <QObject>
#include <QMutex>

#include "RandomDeviate.h"


class TONATIUH_KERNEL ParallelRandomDeviate:  public QObject, public RandomDeviate
{
    Q_OBJECT

public:
    ParallelRandomDeviate(RandomDeviate* rand, QMutex* mutex, ulong arraySize = 100000, QObject* parent = 0);
    virtual ~ParallelRandomDeviate() {}
    void FillArray(double* array, const ulong arraySize);

private:
    RandomDeviate* m_pRand;
    QMutex* m_mutex;
};
