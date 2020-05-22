#pragma once

#include "ShapeRT.h"
#include "kernel/TonatiuhTypes.h"


class TONATIUH_KERNEL ShapeCube: public ShapeRT
{
    SO_NODE_HEADER(ShapeCube);

public:
    ShapeCube();
    static void initClass();

    double getArea() const;
    double getVolume() const;
    BoundingBox getBox() const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;
    bool intersectP(const Ray& ray) const;

    trt::TONATIUH_REAL sizeX;
    trt::TONATIUH_REAL sizeY;
    trt::TONATIUH_REAL sizeZ;

protected:
    ~ShapeCube() {}
    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
    void generatePrimitives(SoAction* action);
};
