#pragma once

#include "kernel/scene/TFactory.h"

class TrackerAbstract;



class TrackerFactory: public TFactory
{
public:
    virtual TrackerAbstract* create() const = 0;
};

Q_DECLARE_INTERFACE(TrackerFactory, "tonatiuh.TrackerFactory")



template<class T>
class TrackerFactoryT: public TrackerFactory
{
public:
    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    void init() const {T::initClass();}
    T* create() const {return new T;}
};
