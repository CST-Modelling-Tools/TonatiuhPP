#pragma once

#include "kernel/scene/TFactory.h"

#include <QVector>
#include <QVariant>

class ShapeAbstract;



class ShapeFactory: public TFactory
{
public:
    virtual ShapeAbstract* create() const = 0;
    virtual ShapeAbstract* create(QVector<QVariant> /*parameters*/) const {return create();}
    virtual bool isFlat() = 0; // better without const?
};

Q_DECLARE_INTERFACE(ShapeFactory, "tonatiuh.ShapeFactory")



template<class T>
class ShapeFactoryT: public ShapeFactory
{
public:
    typedef T ShapeClass;

    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    void init() const {T::initClass();}
    T* create() const {return new T;}
    bool isFlat() {return T::isFlat();}
};
