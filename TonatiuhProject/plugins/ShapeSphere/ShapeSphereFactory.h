#pragma once

#include "ShapeSphere.h"
#include "kernel/raytracing/TShapeFactory.h"

class ShapeSphereFactory: public QObject, public TShapeFactory
{
    Q_OBJECT
    Q_INTERFACES(TShapeFactory)

    Q_PLUGIN_METADATA(IID "tonatiuh.TShapeFactory")

public:
    QString TShapeName() const;
    QIcon TShapeIcon() const;
    ShapeSphere* CreateTShape() const;
    bool IsFlat() {
        return false;
    }
};
