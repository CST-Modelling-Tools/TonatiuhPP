#pragma once

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>

#include "kernel/raytracing/TShape.h"
#include "kernel/raytracing/trt.h"


class ShapeParabolicRectangle: public TShape
{
	SO_NODE_HEADER(ShapeParabolicRectangle);

public:

	ShapeParabolicRectangle();
	static void initClass();
	double GetArea() const;
    double GetVolume() const {
        return 0.0;
    }
	BBox GetBBox() const;

    bool Intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;
    bool IntersectP(const Ray& ray) const;

    Point3D Sample(double u, double v) const;

	trt::TONATIUH_REAL focusLength;
	trt::TONATIUH_REAL widthX;
	trt::TONATIUH_REAL widthZ;
	SoSFEnum activeSide;

    static const char* getClassName() {return "ParabolicRectangle";}
    static const char* getClassIcon() {return ":/ShapeParabolicRectangle.png";}
    const char* getIcon() const {return getClassIcon();}

protected:
    Point3D GetPoint3D (double u, double v) const;
    NormalVector GetNormal(double u, double v) const;

    void computeBBox(SoAction* action, SbBox3f& box, SbVec3f& center);
    void generatePrimitives(SoAction* action);
    ~ShapeParabolicRectangle();
};




#include "kernel/raytracing/TShapeFactory.h"

class ShapeSphereFactory: public QObject, public ShapeFactory<ShapeParabolicRectangle>
{
    Q_OBJECT
    Q_INTERFACES(TShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.TShapeFactory")
};
