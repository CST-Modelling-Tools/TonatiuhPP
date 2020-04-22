#pragma once

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>

#include "kernel/raytracing/TShape.h"
#include "kernel/raytracing/trt.h"


class ShapeParabolicRectangle : public TShape
{
	SO_NODE_HEADER(ShapeParabolicRectangle);

public:
	enum Side{
		INSIDE = 0,
		OUTSIDE   = 1,
	};

	ShapeParabolicRectangle();
	static void initClass();
	double GetArea() const;
    double GetVolume() const {return 0.0;}
	BBox GetBBox() const;
	QString GetIcon() const;

	bool Intersect(const Ray &ray, double *tHit, DifferentialGeometry *dg ) const;
	bool IntersectP( const Ray &ray ) const;

	Point3D Sample( double u, double v ) const;

	trt::TONATIUH_REAL focusLength;
	trt::TONATIUH_REAL widthX;
	trt::TONATIUH_REAL widthZ;
	SoSFEnum activeSide;

protected:
	Point3D GetPoint3D ( double u, double v ) const;
	NormalVector GetNormal( double u, double v ) const;
	bool OutOfRange( double u, double v ) const;

	void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center);
	void generatePrimitives(SoAction *action);
   	~ShapeParabolicRectangle();
};
