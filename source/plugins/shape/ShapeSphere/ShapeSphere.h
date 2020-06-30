#pragma once

#include "kernel/shape/ShapeRT.h"


class ShapeSphere: public ShapeRT
{
    SO_NODE_HEADER(ShapeSphere);

public:
    static void initClass();
    ShapeSphere();

    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
    Vector2D getUV(const Vector3D& p) const;
    Box3D getBox(ProfileRT* profile) const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const;

    NAME_ICON_FUNCTIONS("Sphere", ":/ShapeSphere.png")
    void updateShapeGL(TShapeKit* parent);
};



class ShapeSphereFactory:
    public QObject, public ShapeFactoryT<ShapeSphere>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
