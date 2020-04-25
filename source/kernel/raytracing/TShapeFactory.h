#pragma once
#include <QtPlugin>
#include <QVector>
#include <QVariant>
#include <QIcon>

class QString;
class TShape;


class TShapeFactory
{
public:
    virtual ~TShapeFactory() {}
    virtual QString name() const = 0;
    virtual QIcon icon() const = 0;
    virtual TShape* create() const = 0;
    virtual TShape* create(int /*numberofParameters*/, QVector<QVariant> /*parametersList*/) const
    {
        return create();
    }
    virtual bool isFlat() {return false;}
};


template<class T>
class ShapeFactory: public TShapeFactory
{
public:

    QString name() const
    {
        return T::getClassName();
    }

    QIcon icon() const
    {
        return QIcon(T::getClassIcon());
    }

    T* create() const
    {
        static bool first = true;
        if (first) {
            T::initClass();
            first = false;
        }
        return new T;
    }
};

Q_DECLARE_INTERFACE(TShapeFactory, "tonatiuh.TShapeFactory")
