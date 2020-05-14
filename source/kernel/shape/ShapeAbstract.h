#pragma once

#include "kernel/scene/TAbstract.h"

#include <Inventor/nodes/SoShape.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>

struct Vector3D;
struct BoundingBox;
class Ray;
struct DifferentialGeometry;
class QSize;


class TONATIUH_KERNEL ShapeAbstract: public SoShape
{
    SO_NODE_ABSTRACT_HEADER(ShapeAbstract);

public:
    static void initClass();

    virtual double getArea() const {return -1.;}
    virtual double getVolume() const {return 0.;}
    virtual BoundingBox getBox() const = 0;

    // with computing dg
    virtual bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const = 0;
    // without computing dg
    virtual bool intersectP(const Ray& ray) const;

    enum Side {
        back = 0, // FRONT, INSIDE
        front = 1 // BACK, OUTSIDE
    };

    SoSFEnum activeSide;

    NAME_ICON_FUNCTIONS("X", ":/ShapeX.png")
    static bool isFlat() {return false;}

protected:
    virtual void computeBBox(SoAction* action, SbBox3f& box, SbVec3f& center);

    virtual bool isInside(double u, double v) const;
    virtual Vector3D getPoint(double u, double v) const = 0;
    virtual Vector3D getNormal(double u, double v) const = 0;
    virtual void generatePrimitives(SoAction* action) = 0;
    void generateQuads(SoAction* action, const QSize& dims, bool reverseNormals = false, bool reverseClock = false);

    ShapeAbstract() {}
    ~ShapeAbstract() {}
};
