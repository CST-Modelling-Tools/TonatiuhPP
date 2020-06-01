#pragma once

#include "kernel/shape/ShapeRT.h"


class TONATIUH_KERNEL ShapePlanar: public ShapeRT
{
    SO_NODE_HEADER(ShapePlanar);

public:
    static void initClass();
    ShapePlanar();

    double getArea() const;
    BoundingBox getBox() const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;

    SoSFDouble sizeX;
    SoSFDouble sizeY;
	SoSFEnum activeSide;

    NAME_ICON_FUNCTIONS("Planar", ":/images/ShapePlanar.png")
    static bool isFlat() {return true;}

    void updateShapeGL(TShapeKit* parent);

protected:
    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
	void generatePrimitives(SoAction *action);
};
