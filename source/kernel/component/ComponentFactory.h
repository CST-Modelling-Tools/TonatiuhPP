#pragma once

#include "kernel/scene/TFactory.h"

class TSeparatorKit;



class ComponentFactory: public TFactory
{
public:
    virtual TSeparatorKit* CreateTComponent(PluginManager* pPluginManager) const = 0;
    virtual TSeparatorKit* CreateTComponent(PluginManager* pPluginManager, int numberofParameters, QVector<QVariant> parametersList) const = 0;
};

Q_DECLARE_INTERFACE(ComponentFactory, "tonatiuh.ComponentFactory")
