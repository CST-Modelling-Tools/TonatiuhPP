#pragma once

#include "kernel/scene/TAbstract.h"

#include <Inventor/nodes/SoShape.h>

struct Vector3D;
struct BoundingBox;
class Ray;
struct DifferentialGeometry;
class QSize;
class TShapeKit;


class TONATIUH_KERNEL ShapeRT: public SoNode
{
    SO_NODE_ABSTRACT_HEADER(ShapeRT);

public:
    static void initClass();

    virtual double getArea() const {return -1.;}
    virtual double getVolume() const {return 0.;}
    virtual BoundingBox getBox() const = 0;

    // with computing dg
    virtual bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const = 0;
    // without computing dg
    virtual bool intersectP(const Ray& ray) const {return intersect(ray, 0, 0);}

    enum Side {
        back = 0, // FRONT, INSIDE
        front = 1 // BACK, OUTSIDE
    };

    SoSFEnum activeSide;

    NAME_ICON_FUNCTIONS("X", ":/ShapeX.png")

    virtual void updateShapeGL(TShapeKit* /*parent*/) {}

protected:
    void computeBBox(SoAction* action, SbBox3f& box, SbVec3f& center);

    virtual Vector3D getPoint(double u, double v) const = 0;
    virtual Vector3D getNormal(double u, double v) const = 0;
    void makeQuadMesh(TShapeKit* parent, const QSize& dims, bool reverseNormals = false, bool reverseClock = false);

    ShapeRT() {}
    ~ShapeRT() {}
};



#include "kernel/scene/TFactory.h"

#include <QVector>
#include <QVariant>

class ShapeFactory: public TFactory
{
public:
    virtual ShapeRT* create() const = 0;
    virtual ShapeRT* create(QVector<QVariant> /*parameters*/) const {return create();}
};

Q_DECLARE_INTERFACE(ShapeFactory, "tonatiuh.ShapeFactory")


template<class T>
class ShapeFactoryT: public ShapeFactory
{
public:
    typedef T ShapeClass;

    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    void init() const {T::initClass();}
    T* create() const {return new T;}
};
