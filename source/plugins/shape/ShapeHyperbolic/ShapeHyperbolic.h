#pragma once

#include "kernel/shape/ShapeRT.h"


class ShapeHyperbolic: public ShapeRT
{
    SO_NODE_HEADER(ShapeHyperbolic);

public:
    static void initClass();
    ShapeHyperbolic();

    Box3D getBox(ProfileRT* profile) const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const;

    SoSFDouble focusX;
    SoSFDouble focusY;

    NAME_ICON_FUNCTIONS("Hyperbolic", ":/ShapeHyperbolic.png")
    void updateShapeGL(TShapeKit* parent);

protected:
    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
};



class ShapeHyperbolicFactory:
    public QObject, public ShapeFactoryT<ShapeHyperbolic>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
