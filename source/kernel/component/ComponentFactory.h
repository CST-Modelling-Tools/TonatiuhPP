#pragma once

#include "kernel/scene/TFactory.h"

class TSeparatorKit;
class PluginManager;



class ComponentFactory: public TFactory
{
public:
    virtual TSeparatorKit* create(PluginManager* /*pm*/) const {return 0;}
    virtual TSeparatorKit* create(PluginManager* /*pm*/, int /*numberofParameters*/, QVector<QVariant> /*parametersList*/) const {return 0;}
};

Q_DECLARE_INTERFACE(ComponentFactory, "tonatiuh.ComponentFactory")
