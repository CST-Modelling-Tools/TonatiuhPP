#pragma once

#include <Inventor/fields/SoSFDouble.h>
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
    Point3D Sample(double u1, double u2) const;

    trt::TONATIUH_REAL m_widthX;
    trt::TONATIUH_REAL m_widthY;
    trt::TONATIUH_REAL m_widthZ;

protected:
    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
    void generatePrimitives(SoAction *action);
    ~ShapeCube() {}

private:
    static SbVec3d frontNormal;
    static SbVec3d rearNormal;
    static SbVec3d leftNormal;
    static SbVec3d rightNormal;
    static SbVec3d baseNormal;
    static SbVec3d topNormal;
};
