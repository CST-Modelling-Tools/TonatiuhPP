#pragma once

#include <QtPlugin>
#include <QIcon>

class QString;
class QWidget;
class TShape;
class TSunShape;

class TSunShapeFactory
{
public:
    virtual ~TSunShapeFactory() {}
    virtual QString TSunShapeName() const  = 0;
    virtual QIcon TSunShapeIcon() const = 0;
    virtual TSunShape* create() const = 0;
};

Q_DECLARE_INTERFACE( TSunShapeFactory, "tonatiuh.TSunShapeFactory")


template<class T>
class SunFactory: public TSunShapeFactory
{
public:

    QString TSunShapeName() const
    {
        return T::getClassName();
    }

    QIcon TSunShapeIcon() const
    {
        return QIcon(QString(":/Sun%1.png").arg(T::getClassName()));
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
