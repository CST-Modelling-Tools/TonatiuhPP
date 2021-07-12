#pragma once

#include "kernel/TonatiuhKernel.h"

#include <QMutex>

#include "Random.h"


class TONATIUH_KERNEL RandomParallel: public Random
{

public:
    RandomParallel(Random* rand, QMutex* mutex, ulong size = 100'000);

    void FillArray(std::vector<double>& array);

protected:
    Random* m_rand;
    QMutex* m_mutex;
};
