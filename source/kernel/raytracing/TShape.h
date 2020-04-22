#pragma once

#include <Inventor/nodes/SoShape.h>

struct BBox;
struct DifferentialGeometry;
struct NormalVector;
struct Point3D;
class QString;
class Ray;

class TShape: public SoShape
{
    SO_NODE_ABSTRACT_HEADER(TShape);

public:
    static void initClass();

    virtual bool IntersectP(const Ray& objectRay) const = 0;
    virtual bool Intersect(const Ray& objectRay, double* tHit, DifferentialGeometry* dg) const = 0;
    virtual double GetArea() const = 0;
    virtual double GetVolume() const = 0;
    virtual BBox GetBBox() const = 0;
    virtual QString GetIcon() const = 0;
    virtual Point3D Sample(double u, double v) const = 0;

    static const char* getClassName() {return "Shape";}

protected:
    virtual void computeBBox(SoAction* action, SbBox3f& box, SbVec3f& center) = 0;
    virtual void generatePrimitives(SoAction* action) = 0;

    TShape();
    ~TShape();
};
