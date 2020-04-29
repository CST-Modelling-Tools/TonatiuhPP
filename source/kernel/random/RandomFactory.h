#pragma once

#include "kernel/raytracing/TFactory.h"
#include <QTime>

class RandomDeviate;



class RandomFactory: public TFactory
{
public:
    virtual RandomDeviate* create() const = 0;
};

Q_DECLARE_INTERFACE(RandomFactory, "tonatiuh.RandomFactory")



template<class T>
class RandomFactoryT: public RandomFactory
{
public:
    QString name() const {return T::getClassName();}

    T* create() const {
        ulong seed = QTime::currentTime().msec();
        return new T(seed);
    }
};
