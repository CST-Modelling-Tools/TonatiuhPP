#pragma once

#include <QtPlugin>

class QString;
class QIcon;
class TTransmissivity;

class TTransmissivityFactory
{
public:
    virtual QString name() const = 0;
    virtual QIcon icon() const = 0;
    virtual TTransmissivity* create() const = 0;
};

Q_DECLARE_INTERFACE(TTransmissivityFactory, "tonatiuh.TTransmissivityFactory")
