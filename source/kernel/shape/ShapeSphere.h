#pragma once

#include "kernel/shape/ShapeRT.h"


class TONATIUH_KERNEL ShapeSphere: public ShapeRT
{
    SO_NODE_HEADER(ShapeSphere);

public:
    static void initClass();
    ShapeSphere();

    vec3d getPoint(double u, double v) const;
    vec3d getNormal(double u, double v) const;
    double getRadiusMin(double u, double v) const;
    ProfileRT* getDefaultProfile() const;
    void updateShapeGL(TShapeKit* parent);

    vec2d getUV(const vec3d& p) const;
    Box3D getBox(ProfileRT* profile) const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const;

    NAME_ICON_FUNCTIONS("Sphere", ":/shape/ShapeSphere.png")
};
