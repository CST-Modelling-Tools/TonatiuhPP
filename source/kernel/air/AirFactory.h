#pragma once

#include "kernel/raytracing/TFactory.h"

class AirAbstract;



class AirFactory: public TFactory
{
public:
    virtual QString name() const = 0;
    virtual QIcon icon() const = 0;
    virtual AirAbstract* create() const = 0;
};
Q_DECLARE_INTERFACE(AirFactory, "tonatiuh.AirFactory")



template<class T>
class AirFactoryT: public AirFactory
{
public:
    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    void init() const {T::initClass();}
    T* create() const {return new T;}
};
