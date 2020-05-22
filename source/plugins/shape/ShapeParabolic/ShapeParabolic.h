#pragma once

#include "kernel/shape/ShapeRT.h"
#include "kernel/TonatiuhTypes.h"


class ShapeParabolic: public ShapeRT
{
    SO_NODE_HEADER(ShapeParabolic);

public:
    static void initClass();
    ShapeParabolic();

    BoundingBox getBox() const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;

    SoSFDouble focusX;
    SoSFDouble focusY;
    SoSFDouble sizeX;
    SoSFDouble sizeY;

    NAME_ICON_FUNCTIONS("Parabolic", ":/ShapeParabolic.png")

protected:
    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
    void generatePrimitives(SoAction* action);
};



class ShapeParabolicFactory:
    public QObject, public ShapeFactoryT<ShapeParabolic>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
