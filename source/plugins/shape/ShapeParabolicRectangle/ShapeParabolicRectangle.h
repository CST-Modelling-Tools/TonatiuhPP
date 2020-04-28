#pragma once

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>

#include "kernel/shape/ShapeAbstract.h"
#include "kernel/raytracing/trt.h"


class ShapeParabolicRectangle: public ShapeAbstract
{
	SO_NODE_HEADER(ShapeParabolicRectangle);

public:

	ShapeParabolicRectangle();
	static void initClass();

	BBox GetBBox() const;

    bool Intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;

    Point3D Sample(double u, double v) const;

	trt::TONATIUH_REAL focusLength;
	trt::TONATIUH_REAL widthX;
	trt::TONATIUH_REAL widthZ;
	SoSFEnum activeSide;

    NAME_ICON_FUNCTIONS("ParabolicRectangle", ":/ShapeParabolicRectangle.png")

protected:
    Point3D GetPoint3D (double u, double v) const;
    Vector3D GetNormal(double u, double v) const;
    void generatePrimitives(SoAction* action);
};




#include "kernel/shape/ShapeFactory.h"

class ShapeSphereFactory: public QObject, public ShapeFactoryT<ShapeParabolicRectangle>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
