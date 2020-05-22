#pragma once

#include "kernel/TonatiuhKernel.h"
#include <QObject>
#include <QMutex>

#include "Random.h"


class TONATIUH_KERNEL RandomParallel: public QObject, public Random
{
    Q_OBJECT

public:
    RandomParallel(Random* rand, QMutex* mutex, ulong arraySize = 100000, QObject* parent = 0);
    virtual ~RandomParallel() {}
    void FillArray(double* array, const ulong arraySize);

private:
    Random* m_pRand;
    QMutex* m_mutex;
};
