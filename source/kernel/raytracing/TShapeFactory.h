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
    virtual TShape* CreateTShape() const = 0;
    virtual TShape* CreateTShape(int /*numberofParameters*/, QVector<QVariant> /*parametersList*/) const
    {
        return CreateTShape();
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

    T* CreateTShape() const
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
