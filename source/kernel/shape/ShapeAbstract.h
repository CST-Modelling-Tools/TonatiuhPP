#pragma once

#include "kernel/TonatiuhKernel.h"
#include "kernel/scene/TAbstract.h"
#include <Inventor/nodes/SoShape.h>

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

    virtual bool IntersectP(const Ray& ray) const;
    virtual bool Intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const = 0;

    virtual double GetArea() const {return -1.;}
    virtual double GetVolume() const {return 0.;}
    virtual BBox GetBBox() const = 0;

    enum Side {
        INSIDE = 0,
        OUTSIDE = 1,
    };

    NAME_ICON_FUNCTIONS("X", ":/ShapeX.png")

    protected:
    virtual void computeBBox(SoAction* action, SbBox3f& box, SbVec3f& center);

    virtual bool OutOfRange(double u, double v) const;
    virtual Point3D GetPoint3D(double u, double v) const = 0;
    virtual Vector3D GetNormal(double u, double v) const = 0;
    virtual void generatePrimitives(SoAction* action) = 0;
    void generateQuads(SoAction* action, const QSize& dims, bool reverseNormals = false, bool reverseClock = false);

    ShapeAbstract() {}
    ~ShapeAbstract() {}
};
