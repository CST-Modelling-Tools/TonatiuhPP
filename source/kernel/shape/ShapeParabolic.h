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
    double getStepHint(double u, double v) const;

    void updateShapeGL(TShapeKit* parent);
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const;

    SoSFDouble fX;
    SoSFDouble fY;

    NAME_ICON_FUNCTIONS("Parabolic", ":/shape/ShapeParabolic.png")
};
