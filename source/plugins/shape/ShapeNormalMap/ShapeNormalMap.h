#pragma once

#include "kernel/shape/ShapeRT.h"


class ShapeNormalMap: public ShapeRT
{
    SO_NODE_HEADER(ShapeNormalMap);

public:
    static void initClass();
    ShapeNormalMap();

    vec3d getPoint(double u, double v) const;
    vec3d getNormal(double u, double v) const;
    Box3D getBox(ProfileRT* profile) const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const;

    SoSFDouble fX;
    SoSFDouble fY;

    NAME_ICON_FUNCTIONS("NormalMap", ":/ShapeNormalMap.png")
    void updateShapeGL(TShapeKit* parent);
};



class ShapeNormalMapFactory:
    public QObject, public ShapeFactoryT<ShapeNormalMap>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
