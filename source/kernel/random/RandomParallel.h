#pragma once

#include "kernel/TonatiuhKernel.h"
#include <QObject>
#include <QMutex>

#include "RandomAbstract.h"


class TONATIUH_KERNEL RandomParallel: public QObject, public RandomAbstract
{
    Q_OBJECT

public:
    RandomParallel(RandomAbstract* rand, QMutex* mutex, ulong arraySize = 100000, QObject* parent = 0);
    virtual ~RandomParallel() {}
    void FillArray(double* array, const ulong arraySize);

private:
    RandomAbstract* m_pRand;
    QMutex* m_mutex;
};
