#pragma once

#include <QtPlugin>
#include <QIcon>
#include <QTime>

class QString;
class RandomDeviate;

//!  RandomDeviateFactory is the interface for random generators plugins.
/*!
   A random generator plugin must implement the following interface to load as a valid plugin for Toantiuh.
 */

class RandomDeviateFactory
{
public:
    virtual QString RandomDeviateName() const = 0;
    virtual QIcon RandomDeviateIcon() const {return QIcon();}
    virtual RandomDeviate* CreateRandomDeviate() const = 0;
};

Q_DECLARE_INTERFACE(RandomDeviateFactory, "tonatiuh.RandomDeviateFactory")


template<class T>
class RandomFactory: public RandomDeviateFactory
{
public:

    QString RandomDeviateName() const
    {
        return T::getClassName();
    }

    T* CreateRandomDeviate() const
    {
        ulong seed = QTime::currentTime().msec();
        return new T(seed);
        //return new RandomMersenneTwister(123);
    }
};
