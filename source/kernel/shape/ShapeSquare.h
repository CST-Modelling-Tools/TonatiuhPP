#pragma once

#include "kernel/TonatiuhKernel.h"
#include <Inventor/fields/SoSFDouble.h>

#include "ShapeAbstract.h"

class Transform;


class TONATIUH_KERNEL ShapeSquare: public ShapeAbstract
{
    SO_NODE_HEADER(ShapeSquare);

public:
    static void initClass();
    ShapeSquare();

    double getArea() const;
    BBox getBox() const;
    QString GetIcon() const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;
    bool intersectP(const Ray& ray) const;

    SoSFDouble m_sideLength;

protected:
    ~ShapeSquare() {}

    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
    void generatePrimitives(SoAction* action);
};
