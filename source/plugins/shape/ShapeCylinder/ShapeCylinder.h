#pragma once

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoShape.h>

#include "kernel/shape/ShapeAbstract.h"
#include "kernel/raytracing/trt.h"

class ShapeCylinder: public ShapeAbstract
{
	SO_NODE_HEADER(ShapeCylinder);

public:
    ShapeCylinder();
	static void initClass();
	double GetArea() const;
	double GetVolume() const;
	BBox GetBBox() const;

    bool Intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;

	trt::TONATIUH_REAL radius;
	trt::TONATIUH_REAL length;
	trt::TONATIUH_REAL phiMax;
	SoSFEnum activeSide;

    NAME_ICON_FUNCTIONS("Cylinder", ":/ShapeCylinder.png")

protected:
    Point3D GetPoint3D(double u, double v) const;
    Vector3D GetNormal(double u, double v) const;

    void generatePrimitives(SoAction* action);
	virtual ~ShapeCylinder();
};



#include "kernel/shape/ShapeFactory.h"

class ShapeSphereFactory: public QObject, public ShapeFactoryT<ShapeCylinder>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
