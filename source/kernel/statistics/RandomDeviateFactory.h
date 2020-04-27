#pragma once

#include "kernel/raytracing/TFactory.h"

#include <QTime>

class RandomDeviate;



class RandomDeviateFactory: public TFactory
{
public:
    virtual RandomDeviate* create() const = 0;
};

Q_DECLARE_INTERFACE(RandomDeviateFactory, "tonatiuh.RandomDeviateFactory")



template<class T>
class RandomFactory: public RandomDeviateFactory
{
public:
    QString name() const {return T::getClassName();}

    T* create() const {
        ulong seed = QTime::currentTime().msec();
        return new T(seed);
    }
};
