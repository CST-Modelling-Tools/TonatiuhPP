#pragma once

#include "kernel/scene/TAbstract.h"

#include <Inventor/nodes/SoShape.h>
#include <Inventor/fields/SoSFEnum.h>

struct BBox;
struct DifferentialGeometry;
struct Point3D;
struct Vector3D;
class QString;
class QSize;
class Ray;


class TONATIUH_KERNEL ShapeAbstract: public SoShape
{
    SO_NODE_ABSTRACT_HEADER(ShapeAbstract);

public:
    static void initClass();

    virtual double getArea() const {return -1.;}
    virtual double getVolume() const {return 0.;}
    virtual BBox getBox() const = 0;

    // with computing dg
    virtual bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const = 0;
    // without computing dg
    virtual bool intersectP(const Ray& ray) const;

    enum Side {
        INSIDE = 0,
        OUTSIDE = 1,
    };


    NAME_ICON_FUNCTIONS("X", ":/ShapeX.png")

protected:
    virtual void computeBBox(SoAction* action, SbBox3f& box, SbVec3f& center);

    virtual bool isInside(double u, double v) const;
    virtual Point3D getPoint(double u, double v) const = 0;
    virtual Vector3D getNormal(double u, double v) const = 0;
    virtual void generatePrimitives(SoAction* action) = 0;
    void generateQuads(SoAction* action, const QSize& dims, bool reverseNormals = false, bool reverseClock = false);

    ShapeAbstract() {}
    ~ShapeAbstract() {}
};
