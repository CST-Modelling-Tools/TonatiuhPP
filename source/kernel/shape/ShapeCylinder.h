#pragma once

#include "kernel/shape/ShapeRT.h"


class TONATIUH_KERNEL ShapeCylinder: public ShapeRT
{
	SO_NODE_HEADER(ShapeCylinder);

public:
    enum Caps {
        none = 0,
        bottom,
        top,
        both
    };

    static void initClass();
    ShapeCylinder();

    ProfileRT* getDefaultProfile() const;

    vec3d getPoint(double u, double v) const;
    vec3d getNormal(double u, double v) const;

    vec2d getUV(const vec3d& p) const;
    Box3D getBox(ProfileRT* profile) const;
    double getStepHint(double u, double v) const;

    void updateShapeGL(TShapeKit* parent);
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const;

    SoSFEnum caps;

    NAME_ICON_FUNCTIONS("Cylinder", ":/shape/ShapeCylinder.png")
};
