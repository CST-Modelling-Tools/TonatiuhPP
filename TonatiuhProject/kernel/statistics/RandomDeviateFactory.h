#pragma once

#include <QtPlugin>

class QString;
class QIcon;
class RandomDeviate;

//!  RandomDeviateFactory is the interface for random generators plugins.
/*!
   A random generator plugin must implement the following interface to load as a valid plugin for Toantiuh.
 */

class RandomDeviateFactory
{
public:
    virtual QString RandomDeviateName() const = 0;
    virtual QIcon RandomDeviateIcon() const = 0;
    virtual RandomDeviate* CreateRandomDeviate() const = 0;
};

Q_DECLARE_INTERFACE(RandomDeviateFactory, "tonatiuh.RandomDeviateFactory")
