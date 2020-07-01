#pragma once

#include "kernel/shape/ShapeRT.h"


class ShapeElliptic: public ShapeRT
{
    SO_NODE_HEADER(ShapeElliptic);

public:
    static void initClass();
    ShapeElliptic();

    vec3d getPoint(double u, double v) const;
    vec3d getNormal(double u, double v) const;
    Box3D getBox(ProfileRT* profile) const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const;

    SoSFDouble aX;
    SoSFDouble aY;
    SoSFDouble aZ;

    NAME_ICON_FUNCTIONS("Elliptic", ":/ShapeElliptic.png")
    void updateShapeGL(TShapeKit* parent);
};



class ShapeEllipticFactory:
    public QObject, public ShapeFactoryT<ShapeElliptic>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};

// main equation
// (x/rX)^2 + (y/rY)^2 + (z/rZ - 1)^2 = 1

// points
// z/rZ = 1 - sqrt[1 - (x/rX)^2 - (y/rY)^2]

// dpdx
// z/rZ = x/rX^2/sqrt[1 - (x/rX)^2 - (y/rY)^2]

// normals
// [x/rx^2, y/rY^2, (z/rZ - 1)/rZ]

// relation to parabola
// z/rZ = [(x/rX)^2 + (y/rY)^2]/[1 + sqrt[1 - (x/rX)^2 - (y/rY)^2]]
// 4*fX*z = x^2
// fX = rX^2/(2*rZ)

// ray substitution
// r = r0 + d*t

// mesh: precomputed curvature (hard) or adaptive subdivision (better)
// https://en.wikipedia.org/wiki/Differential_geometry_of_surfaces
// https://en.wikipedia.org/wiki/Paraboloid

// focusing properties
// ellipse: rays from one focus are directed to another focus
// hyperbola: rays from one focus are directed from another focus
// parabola: rays from infinity

// area  y''s^3/3 vs (1 + y'^2)s^2
// parabola y' = x/(2f), y'' = 1/(2f)
