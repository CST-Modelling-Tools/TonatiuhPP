#pragma once

#include "kernel/raytracing/TFactory.h"

#include <QVector>
#include <QVariant>

class TShape;



class TONATIUH_KERNEL TShapeFactory: public TFactory
{
public:
    virtual TShape* create() const = 0;
    virtual TShape* create(QVector<QVariant> /*parameters*/) const {return create();}
    virtual bool isFlat() {return false;}
};
Q_DECLARE_INTERFACE(TShapeFactory, "tonatiuh.TShapeFactory")



template<class T>
class ShapeFactory: public TShapeFactory
{
public:
    typedef T ShapeClass;

    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    void init() const {T::initClass();}
    T* create() const {return new T;}
};
