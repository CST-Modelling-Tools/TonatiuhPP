#pragma once

#include "kernel/raytracing/TFactory.h"

class TTransmissivity;



class TTransmissivityFactory: public TFactory
{
public:
    virtual QString name() const = 0;
    virtual QIcon icon() const = 0;
    virtual TTransmissivity* create() const = 0;
};
Q_DECLARE_INTERFACE(TTransmissivityFactory, "tonatiuh.TTransmissivityFactory")



template<class T>
class AirFactory: public TTransmissivityFactory
{
public:
    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    void init() const {T::initClass();}
    T* create() const {return new T;}
};
