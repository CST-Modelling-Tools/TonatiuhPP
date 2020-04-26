#pragma once

#include "kernel/TonatiuhKernel.h"
#include <Inventor/nodes/SoShape.h>

struct BBox;
struct DifferentialGeometry;
struct NormalVector;
struct Point3D;
class QString;
class Ray;

class TONATIUH_KERNEL TShape: public SoShape
{
    SO_NODE_ABSTRACT_HEADER(TShape);

public:
    enum Side {
        INSIDE = 0,
        OUTSIDE = 1,
    };

    static void initClass();

    virtual bool IntersectP(const Ray& objectRay) const = 0;
    virtual bool Intersect(const Ray& objectRay, double* tHit, DifferentialGeometry* dg) const = 0;
    virtual double GetArea() const = 0;
    virtual double GetVolume() const = 0;
    virtual BBox GetBBox() const = 0;
    virtual Point3D Sample(double u, double v) const;

    static const char* getClassName() {return "X";}
    static const char* getClassIcon() {return ":/ShapeX.png";}
    virtual const char* getIcon() const {return getClassIcon();}

protected:
    virtual Point3D GetPoint3D(double u, double v) const = 0;
    bool OutOfRange(double u, double v) const;
    virtual void computeBBox(SoAction* action, SbBox3f& box, SbVec3f& center) = 0;
    virtual void generatePrimitives(SoAction* action) = 0;

    TShape() {}
    ~TShape() {}
};
