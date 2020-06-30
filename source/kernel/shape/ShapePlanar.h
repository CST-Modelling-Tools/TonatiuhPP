#pragma once

#include "kernel/shape/ShapeRT.h"


class TONATIUH_KERNEL ShapePlanar: public ShapeRT
{
    SO_NODE_HEADER(ShapePlanar);

public:
    static void initClass();
    ShapePlanar();

    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const;

    NAME_ICON_FUNCTIONS("Planar", ":/images/ShapePlanar.png")
    void updateShapeGL(TShapeKit* parent);
};
