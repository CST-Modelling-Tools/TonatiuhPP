#pragma once

#include "kernel/shape/ShapeRT.h"


class TONATIUH_KERNEL ShapePlane: public ShapeRT
{
    SO_NODE_HEADER(ShapePlane);

public:
    static void initClass();
    ShapePlane();

    double getArea() const;
    BoundingBox getBox() const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;

    SoSFDouble sizeX;
    SoSFDouble sizeY;
	SoSFEnum activeSide;

    NAME_ICON_FUNCTIONS("Plane", ":/images/ShapePlane.png")
    static bool isFlat() {return true;}

protected:
    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
	void generatePrimitives(SoAction *action);
};
