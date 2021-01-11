#pragma once

#include "kernel/shape/ShapeRT.h"


class TONATIUH_KERNEL ShapeSphere: public ShapeRT
{
    SO_NODE_HEADER(ShapeSphere);

public:
    static void initClass();
    ShapeSphere();

    ProfileRT* getDefaultProfile() const;

    vec3d getPoint(double u, double v) const;
    vec3d getNormal(double u, double v) const;

    vec2d getUV(const vec3d& p) const;
    Box3D getBox(ProfileRT* profile) const;
    double getStepHint(double u, double v) const;

    void updateShapeGL(TShapeKit* parent);
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const;

    NAME_ICON_FUNCTIONS("Sphere", ":/shape/ShapeSphere.png")
};
