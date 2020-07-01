#pragma once

#include "kernel/shape/ShapeRT.h"


class ShapeMesh: public ShapeRT
{
    SO_NODE_HEADER(ShapeMesh);

public:
    static void initClass();
    ShapeMesh();

    vec3d getPoint(double u, double v) const;
    vec3d getNormal(double u, double v) const;
    Box3D getBox(ProfileRT* profile) const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const;

    SoSFDouble fX;
    SoSFDouble fY;

    NAME_ICON_FUNCTIONS("Mesh", ":/ShapeMesh.png")
    void updateShapeGL(TShapeKit* parent);
};



class ShapeMeshFactory:
    public QObject, public ShapeFactoryT<ShapeMesh>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
