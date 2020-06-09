#pragma once

#include "kernel/shape/ShapeRT.h"


class ShapeCylinder: public ShapeRT
{
	SO_NODE_HEADER(ShapeCylinder);

public:
    static void initClass();
    ShapeCylinder();

    Box3D getBox(ProfileRT* aperture) const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const;

    NAME_ICON_FUNCTIONS("Cylinder", ":/ShapeCylinder.png")
    void updateShapeGL(TShapeKit* parent);

protected:
    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
};



class ShapeCylinderFactory: public QObject, public ShapeFactoryT<ShapeCylinder>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
