#pragma once

#include "kernel/shape/ShapeRT.h"


class TONATIUH_KERNEL ShapeParabolic: public ShapeRT
{
    SO_NODE_HEADER(ShapeParabolic);

public:
    static void initClass();
    ShapeParabolic();

    vec3d getPoint(double u, double v) const;
    vec3d getNormal(double u, double v) const;
    Box3D getBox(ProfileRT* profile) const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const;

    SoSFDouble fX;
    SoSFDouble fY;

    NAME_ICON_FUNCTIONS("Parabolic", ":/shape/ShapeParabolic.png")
    void updateShapeGL(TShapeKit* parent);
};
