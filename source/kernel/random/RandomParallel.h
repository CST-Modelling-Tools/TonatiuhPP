#pragma once

#include "kernel/TonatiuhKernel.h"

#include <QObject>
#include <QMutex>

#include "Random.h"


class TONATIUH_KERNEL RandomParallel: public QObject, public Random
{
    Q_OBJECT

public:
    RandomParallel(Random* rand, QMutex* mutex, ulong size = 100'000, QObject* parent = 0);
    virtual ~RandomParallel() {}
    void FillArray();

private:
    Random* m_rand;
    QMutex* m_mutex;
};
