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

    bool Intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;

    double GetArea() const;
    double GetVolume() const;
    BBox GetBBox() const;

    static const char* getClassName() {return "Sphere";}
    static const char* getClassIcon() {return ":/ShapeSphere.png";}
    const char* getIcon() const {return getClassIcon();}

protected:
    ~ShapeSphere();

    Point3D GetPoint3D(double u, double v) const;
    Vector3D GetNormal(double u, double v) const;
    void generatePrimitives(SoAction* action);

    static void update_radius(void* data, SoSensor*);
    static void update_yMin(void* data, SoSensor*);
    static void update_yMax(void* data, SoSensor*);
    static void update_phiMax(void* data, SoSensor*);

public:
    trt::TONATIUH_REAL radius;
    trt::TONATIUH_REAL yMax;
    trt::TONATIUH_REAL yMin;
    trt::TONATIUH_REAL phiMax;
    SoSFEnum activeSide;

private:
    double m_radiusOld;
    double m_yMaxOld;
    double m_yMinOld;

    SoFieldSensor* m_sensor_radius;
    SoFieldSensor* m_sensor_yMin;
    SoFieldSensor* m_sensor_yMax;
    SoFieldSensor* m_sensor_phiMax;
};



#include "kernel/shape/ShapeFactory.h"

class ShapeSphereFactory:
    public QObject, public ShapeFactoryT<ShapeSphere>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
