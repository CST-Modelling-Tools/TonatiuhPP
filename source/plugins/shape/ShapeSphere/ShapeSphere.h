#pragma once

#include "kernel/shape/ShapeAbstract.h"
#include "kernel/TonatiuhTypes.h"

class SoFieldSensor;
class SoSensor;


class ShapeSphere: public ShapeAbstract
{
    SO_NODE_HEADER(ShapeSphere);

public:
    static void initClass();
    ShapeSphere();

    double getArea() const;
    double getVolume() const;
    BBox getBox() const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;

    trt::TONATIUH_REAL radius;
    trt::TONATIUH_REAL phiMax;
    trt::TONATIUH_REAL alphaMin;
    trt::TONATIUH_REAL alphaMax;

    NAME_ICON_FUNCTIONS("Sphere", ":/ShapeSphere.png")

protected:
    ~ShapeSphere();

    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
    void generatePrimitives(SoAction* action);

    SoFieldSensor* m_sensor_radius;
    SoFieldSensor* m_sensor_alphaMin;
    SoFieldSensor* m_sensor_alphaMax;
    SoFieldSensor* m_sensor_phiMax;

    static void update_radius(void* data, SoSensor*);
    static void update_phiMax(void* data, SoSensor*);
    static void update_alphaMin(void* data, SoSensor*);
    static void update_alphaMax(void* data, SoSensor*);
};



#include "kernel/shape/ShapeFactory.h"

class ShapeSphereFactory:
    public QObject, public ShapeFactoryT<ShapeSphere>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
