#pragma once

#include "kernel/shape/ShapeRT.h"


class ShapeParabolic: public ShapeRT
{
    SO_NODE_HEADER(ShapeParabolic);

public:
    static void initClass();
    ShapeParabolic();

    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
    Box3D getBox(ProfileRT* profile) const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const;

    SoSFDouble fX;
    SoSFDouble fY;

    NAME_ICON_FUNCTIONS("Parabolic", ":/ShapeParabolic.png")
    void updateShapeGL(TShapeKit* parent);
};



class ShapeParabolicFactory:
    public QObject, public ShapeFactoryT<ShapeParabolic>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
