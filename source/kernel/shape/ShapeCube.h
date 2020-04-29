#pragma once

#include <Inventor/fields/SoSFDouble.h>
#include "ShapeAbstract.h"
#include "kernel/TonatiuhTypes.h"


class TONATIUH_KERNEL ShapeCube: public ShapeAbstract
{
    SO_NODE_HEADER(ShapeCube);

public:
    ShapeCube( );
    static void initClass();
    double GetArea() const;
    double GetVolume() const;
    BBox GetBBox() const;
    QString GetIcon() const;

    bool Intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;
    bool IntersectP(const Ray& ray) const;
    Point3D Sample(double u1, double u2) const;

    trt::TONATIUH_REAL m_width;
    trt::TONATIUH_REAL m_height;
    trt::TONATIUH_REAL m_depth;

protected:
    Point3D GetPoint3D (double u, double v) const;
    Vector3D GetNormal(double u, double v) const;

    void computeBBox( SoAction* action, SbBox3f& box, SbVec3f& center);
    void generatePrimitives(SoAction *action);
    virtual ~ShapeCube();

private:
    // Face NormalVectors. These are static because they are computed
    // once and are shared by all instances
    static SbVec3d frontNormal, rearNormal;
    static SbVec3d leftNormal, rightNormal;
    static SbVec3d baseNormal, topNormal;
};
