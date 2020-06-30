#pragma once

#include "kernel/scene/TAbstract.h"

#include <Inventor/nodes/SoShape.h>

struct Vector2D;
struct Vector3D;
struct Box3D;
class Ray;
struct DifferentialGeometry;
class QSize;
class TShapeKit;
class ProfileRT;


class TONATIUH_KERNEL ShapeRT: public SoNode
{
    SO_NODE_ABSTRACT_HEADER(ShapeRT);

public:
    static void initClass();

    virtual Vector3D getPoint(double u, double v) const;
    virtual Vector3D getNormal(double u, double v) const;
    virtual Vector2D getUV(const Vector3D& p) const;
    virtual Box3D getBox(ProfileRT* profile) const;

    // with computing dg, ray in local coordinates
    virtual bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const = 0;
    // without computing dg
    virtual bool intersectP(const Ray& ray, ProfileRT* profile) const {return intersect(ray, 0, 0, profile);}

    NAME_ICON_FUNCTIONS("X", ":/ShapeX.png")

    virtual void updateShapeGL(TShapeKit* /*parent*/) {}

protected:
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
