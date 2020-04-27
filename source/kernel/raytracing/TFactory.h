#pragma once

#include "kernel/TonatiuhKernel.h"

#include <QtPlugin>
#include <QIcon>

class QString;


class TFactory
{
public:
    virtual ~TFactory() {}
    virtual QString name() const = 0;
    virtual QIcon icon() const {return QIcon();}
    virtual void init() const {}
};
