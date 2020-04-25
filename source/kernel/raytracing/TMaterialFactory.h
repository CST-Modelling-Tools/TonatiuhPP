#pragma once

#include <QtPlugin>
#include <QIcon>

class QString;
class TMaterial;

class TMaterialFactory
{
public:
    virtual ~TMaterialFactory() {}
    virtual QString name() const = 0;
    virtual QIcon icon() const = 0;
    virtual TMaterial* create() const = 0;
};


template<class T>
class MaterialFactory: public TMaterialFactory
{
public:

    QString name() const
    {
        return T::getClassName();
    }

    QIcon icon() const
    {
        return QIcon(QString(":/Material%1.png").arg(T::getClassName()));
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

Q_DECLARE_INTERFACE(TMaterialFactory, "tonatiuh.TMaterialFactory")
