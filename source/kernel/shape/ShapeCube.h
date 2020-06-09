#pragma once

#include "kernel/shape/ShapeRT.h"


class TONATIUH_KERNEL ShapeCube: public ShapeRT
{
    SO_NODE_HEADER(ShapeCube);

public:
    ShapeCube();
    static void initClass();

    Box3D getBox(ProfileRT* aperture) const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const;

    NAME_ICON_FUNCTIONS("Cube", ":/ShapeCube.png")
    void updateShapeGL(TShapeKit* parent);
};
