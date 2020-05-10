#pragma once

#include "ShapeAbstract.h"
#include "kernel/TonatiuhTypes.h"


class TONATIUH_KERNEL ShapeCube: public ShapeAbstract
{
    SO_NODE_HEADER(ShapeCube);

public:
    ShapeCube();
    static void initClass();

    double getArea() const;
    double getVolume() const;
    BBox getBox() const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;
    bool intersectP(const Ray& ray) const;

    trt::TONATIUH_REAL widthX;
    trt::TONATIUH_REAL widthY;
    trt::TONATIUH_REAL widthZ;

protected:
    ~ShapeCube() {}
    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
    void generatePrimitives(SoAction* action);
};
