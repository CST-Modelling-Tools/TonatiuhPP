#pragma once

#include "ShapeCylinder.h"
#include "kernel/raytracing/TShapeFactory.h"


class ShapeCylinderFactory: public QObject, public TShapeFactory
{
    Q_OBJECT
    Q_INTERFACES(TShapeFactory)

    Q_PLUGIN_METADATA(IID "tonatiuh.TShapeFactory")


public:
    QString TShapeName() const;
    QIcon TShapeIcon() const;
    ShapeCylinder* CreateTShape() const;
    bool IsFlat() {
        return false;
    }
};
