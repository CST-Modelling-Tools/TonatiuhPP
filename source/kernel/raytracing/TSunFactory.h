#pragma once

#include "kernel/raytracing/TFactory.h"

class TSunShape;



class TSunFactory: public TFactory
{
public:
    virtual TSunShape* create() const = 0;
};
Q_DECLARE_INTERFACE( TSunFactory, "tonatiuh.TSunFactory")



template<class T>
class SunFactory: public TSunFactory
{
public:
    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    void init() const {T::initClass();}
    T* create() const {return new T;}
};
