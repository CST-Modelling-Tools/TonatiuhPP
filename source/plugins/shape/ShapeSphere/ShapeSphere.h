#pragma once

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>

#include "kernel/raytracing/TShape.h"
#include "kernel/raytracing/trt.h"

class SoFieldSensor;
class SoSensor;

class ShapeSphere: public TShape
{
    SO_NODE_HEADER(ShapeSphere);

public:
    enum Side {
        INSIDE = 0,
        OUTSIDE = 1,
    };

    ShapeSphere();
    static void initClass();
    SoNode* copy(SbBool copyConnections) const;
    double GetArea() const;
    double GetVolume() const;
    BBox GetBBox() const;

    Point3D Sample(double u1, double u2) const;

    bool Intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;
    bool IntersectP(const Ray& ray) const;

    trt::TONATIUH_REAL radius;
    trt::TONATIUH_REAL yMax;
    trt::TONATIUH_REAL yMin;
    trt::TONATIUH_REAL phiMax;
    SoSFEnum activeSide;

    static const char* getClassName() {return "Sphere";}
    static const char* getClassIcon() {return ":/ShapeSphere.png";}
    const char* getIcon() const {return getClassIcon();}

protected:
    static void updateRadius(void* data, SoSensor*);
    static void updateYMin(void* data, SoSensor*);
    static void updateYMax(void* data, SoSensor*);
    static void updatePhiMax(void* data, SoSensor*);

    bool OutOfRange(double u, double v) const;
    Point3D GetPoint3D (double u, double v) const;
    NormalVector GetNormal(double u, double v) const;

    void computeBBox(SoAction* action, SbBox3f& box, SbVec3f& center);
    void generatePrimitives(SoAction* action);
    virtual ~ShapeSphere();

private:
    double m_lastValidRadius;
    double m_lastValidYMax;
    double m_lastValidYMin;

    SoFieldSensor* m_radiusSensor;
    SoFieldSensor* m_yMinSensor;
    SoFieldSensor* m_yMaxSensor;
    SoFieldSensor* m_phiMaxSensor;
};



#include "kernel/raytracing/TShapeFactory.h"

class ShapeSphereFactory: public QObject, public ShapeFactory<ShapeSphere>
{
    Q_OBJECT
    Q_INTERFACES(TShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.TShapeFactory")
};
