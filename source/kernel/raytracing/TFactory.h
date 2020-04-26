#pragma once

#include "kernel/TonatiuhKernel.h"

#include <QtPlugin>
#include <QIcon>

class QString;


class TFactory
{
public:
    virtual ~TFactory() {}
    virtual QString name() const  = 0;
    virtual QIcon icon() const = 0;
    virtual void init() const = 0;
};
