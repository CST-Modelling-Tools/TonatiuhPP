#pragma once

#include "kernel/shape/ShapeRT.h"


class TONATIUH_KERNEL ShapePlanar: public ShapeRT
{
    SO_NODE_HEADER(ShapePlanar);

public:
    static void initClass();
    ShapePlanar();

    NAME_ICON_FUNCTIONS("Planar", ":/shape/ShapePlanar.png")
    void updateShapeGL(TShapeKit* parent);
};
