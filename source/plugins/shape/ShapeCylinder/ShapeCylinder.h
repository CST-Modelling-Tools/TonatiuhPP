#pragma once

#include "kernel/shape/ShapeAbstract.h"
#include "kernel/TonatiuhTypes.h"


class ShapeCylinder: public ShapeAbstract
{
	SO_NODE_HEADER(ShapeCylinder);

public:
    static void initClass();
    ShapeCylinder();

    double getArea() const;
    double getVolume() const;
    BoundingBox getBox() const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;

    SoSFDouble radius;
    SoSFDouble phiMax;
    SoSFDouble sizeZ;

    NAME_ICON_FUNCTIONS("Cylinder", ":/ShapeCylinder.png")

protected:
    ~ShapeCylinder() {}

    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
    void generatePrimitives(SoAction* action);
};



#include "kernel/shape/ShapeFactory.h"

class ShapeCylinderFactory: public QObject, public ShapeFactoryT<ShapeCylinder>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
