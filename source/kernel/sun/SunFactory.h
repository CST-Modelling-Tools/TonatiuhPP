#pragma once

#include "kernel/scene/TFactory.h"

class SunAbstract;



class SunFactory: public TFactory
{
public:
    virtual SunAbstract* create() const = 0;
};

Q_DECLARE_INTERFACE(SunFactory, "tonatiuh.SunFactory")



template<class T>
class SunFactoryT: public SunFactory
{
public:
    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    void init() const {T::initClass();}
    T* create() const {return new T;}
};
