#pragma once

#include "kernel/shape/ShapeRT.h"


class ShapeCone: public ShapeRT
{
    SO_NODE_HEADER(ShapeCone);

public:
    static void initClass();
    ShapeCone();

    vec3d getPoint(double u, double v) const;
    vec3d getNormal(double u, double v) const;
    vec2d getUV(const vec3d& p) const;
    Box3D getBox(ProfileRT* aperture) const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const;

    SoSFDouble dr;

    NAME_ICON_FUNCTIONS("Cone", ":/ShapeCone.png")
    void updateShapeGL(TShapeKit* parent);
};



class ShapeConeFactory: public QObject, public ShapeFactoryT<ShapeCone>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
