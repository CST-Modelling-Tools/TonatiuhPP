#pragma once

#include "kernel/shape/ShapeRT.h"


class ShapeSpherical: public ShapeRT
{
    SO_NODE_HEADER(ShapeSpherical);

public:
    static void initClass();
    ShapeSpherical();

    Box3D getBox(ProfileRT* profile) const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const;

    SoSFDouble radiusX;
    SoSFDouble radiusY;
    SoSFDouble radiusZ;

    NAME_ICON_FUNCTIONS("Spherical", ":/ShapeSpherical.png")
    void updateShapeGL(TShapeKit* parent);

protected:
    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
};



class ShapeSphericalFactory:
    public QObject, public ShapeFactoryT<ShapeSpherical>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
