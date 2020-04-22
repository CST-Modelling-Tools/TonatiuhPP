#pragma once

#include <QtPlugin>
#include <QIcon>

class QString;
class TMaterial;

class TMaterialFactory
{
public:
    virtual ~TMaterialFactory() {}
    virtual QString TMaterialName() const = 0;
    virtual QIcon TMaterialIcon() const = 0;
    virtual TMaterial* CreateTMaterial() const = 0;
};


template<class T>
class MaterialFactory: public TMaterialFactory
{
public:

    QString TMaterialName() const
    {
        return T::getClassName();
    }

    QIcon TMaterialIcon() const
    {
        return QIcon(QString(":/Material%1.png").arg(T::getClassName()));
    }

    T* CreateTMaterial() const
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
