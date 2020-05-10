#pragma once

#include "kernel/shape/ShapeAbstract.h"
#include "kernel/TonatiuhTypes.h"


class ShapePlane: public ShapeAbstract
{
    SO_NODE_HEADER(ShapePlane);

public:
    static void initClass();
    ShapePlane();

    double getArea() const;
    BBox getBox() const;
    bool intersect(const Ray &ray, double *tHit, DifferentialGeometry *dg ) const;

    trt::TONATIUH_REAL sizeX;
    trt::TONATIUH_REAL sizeY;
	SoSFEnum activeSide;

    NAME_ICON_FUNCTIONS("Plane", ":/ShapePlane.png")
    static bool isFlat() {return true;}

protected:
    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
	void generatePrimitives(SoAction *action);
};


#include "kernel/shape/ShapeFactory.h"

class ShapePlaneFactory:
    public QObject, public ShapeFactoryT<ShapePlane>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
