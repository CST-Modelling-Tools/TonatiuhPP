#pragma once

#include <QtPlugin>
#include <QIcon>

class QString;
class TTransmissivity;

class TTransmissivityFactory
{
public:
    virtual QString name() const = 0;
    virtual QIcon icon() const = 0;
    virtual TTransmissivity* create() const = 0;
};

Q_DECLARE_INTERFACE(TTransmissivityFactory, "tonatiuh.TTransmissivityFactory")


template<class T>
class AirFactory: public TTransmissivityFactory
{
public:

    QString name() const
    {
        return T::getClassName();
    }

    QIcon icon() const
    {
        return QIcon(QString(":/Air%1.png").arg(T::getClassName()));
    }

    T* create() const
    {
        static bool first = true;
        if (first) {
            first = false;
            T::initClass();
        }
        return new T;
    }
};
