#pragma once

#include "kernel/node/TNode.h"

struct vec2d;
struct vec3d;
struct Box3D;
class Ray;
struct DifferentialGeometry;
class QSize;
class TShapeKit;
class ProfileRT;


class TONATIUH_KERNEL ShapeRT: public TNode
{
    SO_NODE_ABSTRACT_HEADER(ShapeRT);

public:
    static void initClass();

    virtual ProfileRT* getDefaultProfile() const;

    virtual vec3d getPoint(double u, double v) const;
    virtual vec3d getNormal(double u, double v) const;
    virtual vec2d getUV(const vec3d& p) const;
    virtual double getStepHint(double u, double v) const;
    virtual void updateShapeGL(TShapeKit* /*parent*/) {}

    virtual Box3D getBox(ProfileRT* profile) const;
    // with computing dg, ray in local coordinates
    virtual bool intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const;
    // without computing dg
    virtual bool intersectP(const Ray& ray, ProfileRT* profile) const {return intersect(ray, 0, 0, profile);}

    NAME_ICON_FUNCTIONS("X", ":/ShapeX.png")


protected:
    void makeQuadMesh(TShapeKit* parent, const QSize& dims, bool forceIndexed = false);
};



#include "kernel/node/TFactory.h"

#include <QVector>
#include <QVariant>

class ShapeFactory: public TFactory
{
public:
    ShapeRT* create() const = 0;
    ShapeRT* create(QVector<QVariant> /*parameters*/) const {return create();}
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
