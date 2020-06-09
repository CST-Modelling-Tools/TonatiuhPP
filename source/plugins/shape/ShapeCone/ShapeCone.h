#pragma once

#include "kernel/shape/ShapeRT.h"


class ShapeCone: public ShapeRT
{
    SO_NODE_HEADER(ShapeCone);

public:
    static void initClass();
    ShapeCone();

    Box3D getBox(ProfileRT* aperture) const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const;

    SoSFDouble dr;

    NAME_ICON_FUNCTIONS("Cone", ":/ShapeCone.png")
    void updateShapeGL(TShapeKit* parent);

protected:
    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
};



class ShapeConeFactory: public QObject, public ShapeFactoryT<ShapeCone>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
