#pragma once

#include "kernel/shape/ShapeAbstract.h"
#include "kernel/TonatiuhTypes.h"


class ShapeParabolicRectangle: public ShapeAbstract
{
	SO_NODE_HEADER(ShapeParabolicRectangle);

public:
    static void initClass();
	ShapeParabolicRectangle();

    BBox getBox() const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;

    trt::TONATIUH_REAL focus;
	trt::TONATIUH_REAL widthX;
	trt::TONATIUH_REAL widthZ;

    SoSFEnum activeSide;

    NAME_ICON_FUNCTIONS("ParabolicRectangle", ":/ShapeParabolicRectangle.png")

protected:
    Point3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
    void generatePrimitives(SoAction* action);
};



#include "kernel/shape/ShapeFactory.h"

class ShapeSphereFactory: public QObject, public ShapeFactoryT<ShapeParabolicRectangle>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
