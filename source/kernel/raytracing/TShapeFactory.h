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
    virtual QString TShapeName() const = 0;
    virtual QIcon TShapeIcon() const = 0;
    virtual TShape* create() const = 0;
    virtual TShape* create(int /*numberofParameters*/, QVector<QVariant> /*parametersList*/) const
    {
        return create();
    }
    virtual bool IsFlat() {return false;}
};


template<class T>
class ShapeFactory: public TShapeFactory
{
public:

    QString TShapeName() const
    {
        return T::getClassName();
    }

    QIcon TShapeIcon() const
    {
        return QIcon(QString(":/Shape%1.png").arg(T::getClassName()));
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
