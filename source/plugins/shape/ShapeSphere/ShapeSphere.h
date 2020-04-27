#pragma once

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>

#include "kernel/shape/ShapeAbstract.h"
#include "kernel/raytracing/trt.h"

class SoFieldSensor;
class SoSensor;


class ShapeSphere: public ShapeAbstract
{
    SO_NODE_HEADER(ShapeSphere);

public:
    ShapeSphere();
    static void initClass();
    SoNode* copy(SbBool copyConnections) const;

    double GetArea() const;
    double GetVolume() const;
    BBox GetBBox() const;

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

    Point3D GetPoint3D(double u, double v) const;
    NormalVector GetNormal(double u, double v) const;

    void computeBBox(SoAction* action, SbBox3f& box, SbVec3f& center);
    void generatePrimitives(SoAction* action);
    ~ShapeSphere();

private:
    double m_lastValidRadius;
    double m_lastValidYMax;
    double m_lastValidYMin;

    SoFieldSensor* m_radiusSensor;
    SoFieldSensor* m_yMinSensor;
    SoFieldSensor* m_yMaxSensor;
    SoFieldSensor* m_phiMaxSensor;
};



#include "kernel/shape/ShapeFactory.h"

class ShapeSphereFactory:
    public QObject, public ShapeFactoryT<ShapeSphere>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
