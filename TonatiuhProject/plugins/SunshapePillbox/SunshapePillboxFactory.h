#pragma once
#include <QObject>

#include "SunshapePillbox.h"
#include "kernel/raytracing/TSunShapeFactory.h"


class SunshapePillboxFactory: public QObject, public TSunShapeFactory
{
    Q_OBJECT
    Q_INTERFACES(TSunShapeFactory)

    Q_PLUGIN_METADATA(IID "tonatiuh.TSunShapeFactory")


public:
    QString TSunShapeName() const;
    QIcon TSunShapeIcon() const;
    SunshapePillbox* CreateTSunShape() const;
};
