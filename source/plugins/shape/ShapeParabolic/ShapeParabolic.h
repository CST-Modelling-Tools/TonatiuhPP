#pragma once

#include "kernel/shape/ShapeAbstract.h"
#include "kernel/TonatiuhTypes.h"


class ShapeParabolic: public ShapeAbstract
{
    SO_NODE_HEADER(ShapeParabolic);

public:
    static void initClass();
    ShapeParabolic();

    BBox getBox() const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;

    trt::TONATIUH_REAL focus;
	trt::TONATIUH_REAL widthX;
    trt::TONATIUH_REAL widthY;

    NAME_ICON_FUNCTIONS("Parabolic", ":/ShapeParabolic.png")

protected:
    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
    void generatePrimitives(SoAction* action);
};



#include "kernel/shape/ShapeFactory.h"

class ShapeParabolicFactory:
    public QObject, public ShapeFactoryT<ShapeParabolic>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
