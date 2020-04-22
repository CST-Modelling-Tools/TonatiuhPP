#pragma once

#include "ShapeParabolicRectangle.h"
#include "kernel/raytracing/TShapeFactory.h"

class ShapeParabolicRectangleFactory: public QObject, public TShapeFactory
{
    Q_OBJECT
    Q_INTERFACES(TShapeFactory)

    Q_PLUGIN_METADATA(IID "tonatiuh.TShapeFactory")


public:
    QString TShapeName() const;
    QIcon TShapeIcon() const;
    ShapeParabolicRectangle* CreateTShape() const;
    bool IsFlat() {
        return false;
    }
};
