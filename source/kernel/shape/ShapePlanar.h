#pragma once

#include "kernel/shape/ShapeRT.h"


class TONATIUH_KERNEL ShapePlanar: public ShapeRT
{
    SO_NODE_HEADER(ShapePlanar);

public:
    static void initClass();
    ShapePlanar();

    BoundingBox getBox(ProfileRT* aperture) const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const;

    NAME_ICON_FUNCTIONS("Planar", ":/images/ShapePlanar.png")
    void updateShapeGL(TShapeKit* parent);

protected:
    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
};
