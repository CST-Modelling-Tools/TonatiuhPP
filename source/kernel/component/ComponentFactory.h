#pragma once

#include "kernel/scene/TFactory.h"

class TSeparatorKit;
class PluginManager;



class ComponentFactory: public TFactory
{
public:
    virtual TSeparatorKit* CreateTComponent(PluginManager* pm) const {}
    virtual TSeparatorKit* CreateTComponent(PluginManager* pm, int numberofParameters, QVector<QVariant> parametersList) const {}
};

Q_DECLARE_INTERFACE(ComponentFactory, "tonatiuh.ComponentFactory")
