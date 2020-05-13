#pragma once

#include "kernel/shape/ShapeAbstract.h"
#include "kernel/TonatiuhTypes.h"

class SoNodeSensor;
class SoSensor;


class ShapeSphere: public ShapeAbstract
{
    SO_NODE_HEADER(ShapeSphere);

public:
    static void initClass();
    ShapeSphere();

    double getArea() const;
    double getVolume() const;
    BoundingBox getBox() const;
    bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const;

    SoSFDouble radius;
    SoSFDouble phiMax;
    SoSFDouble alphaMin;
    SoSFDouble alphaMax;

    NAME_ICON_FUNCTIONS("Sphere", ":/ShapeSphere.png")

protected:
    ~ShapeSphere();

    Vector3D getPoint(double u, double v) const;
    Vector3D getNormal(double u, double v) const;
    void generatePrimitives(SoAction* action);

    SoNodeSensor* m_sensor;
    static void update(void* data, SoSensor*);
};



#include "kernel/shape/ShapeFactory.h"

class ShapeSphereFactory:
    public QObject, public ShapeFactoryT<ShapeSphere>
{
    Q_OBJECT
    Q_INTERFACES(ShapeFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.ShapeFactory")
};
