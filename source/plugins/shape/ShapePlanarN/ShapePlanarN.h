#pragma once

#include "kernel/shape/ShapeRT.h"
#include <Inventor/fields/SoSFVec2i32.h>


class ShapePlanarN: public ShapeRT
{
    SO_NODE_HEADER(ShapePlanarN);

public:
    static void initClass();
    ShapePlanarN();

    vec3d getNormal(double u, double v) const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const;

    SoSFVec2f xLimits;
    SoSFVec2f yLimits;
    SoSFVec2i32 dims;
    SoMFVec3f normals;

    SoSFDouble fX;
    SoSFDouble fY;

    NAME_ICON_FUNCTIONS("PlanarN", ":/ShapePlanarN.png")
    void updateShapeGL(TShapeKit* parent);
};



class ShapePlanarNFactory:
    public QObject, public ShapeFactoryT<ShapePlanarN>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
