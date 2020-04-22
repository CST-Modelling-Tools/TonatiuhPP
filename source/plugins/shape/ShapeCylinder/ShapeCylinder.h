#pragma once

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoShape.h>

#include "kernel/raytracing/TShape.h"
#include "kernel/raytracing/trt.h"

class ShapeCylinder: public TShape
{
	SO_NODE_HEADER(ShapeCylinder);

public:
    ShapeCylinder();
	static void initClass();
	double GetArea() const;
	double GetVolume() const;
	BBox GetBBox() const;
	QString GetIcon() const;

    bool Intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;
    bool IntersectP(const Ray& ray) const;

    Point3D Sample(double u, double v) const;

    enum Side {
		INSIDE = 0,
		OUTSIDE   = 1,
	};

	trt::TONATIUH_REAL radius;
	trt::TONATIUH_REAL length;
	trt::TONATIUH_REAL phiMax;
	SoSFEnum activeSide;

protected:
    bool OutOfRange(double u, double v) const;
    Point3D GetPoint3D(double u, double v) const;
    NormalVector GetNormal(double u, double v) const;

    void generatePrimitives(SoAction* action);
    void computeBBox(SoAction* action, SbBox3f& box, SbVec3f& center);
	virtual ~ShapeCylinder();
};
