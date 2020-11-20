#pragma once

#include <QVector>
#include <QSize>

#include "kernel/node/TNode.h"
#include "libraries/math/2D/vec2d.h"
#include "libraries/math/2D/Box2D.h"

struct DifferentialGeometry;
class Random;
class Ray;


class TONATIUH_KERNEL ProfileRT: public TNode
{
    SO_NODE_ABSTRACT_HEADER(ProfileRT);

public:
    static void initClass();

    virtual Box2D getBox() const {return Box2D::UnitCentered;}
    virtual void setBox(const Box2D& /*box*/) {}

    virtual bool isInside(double /*x*/, double /*y*/) const {return true;}
    virtual QVector<vec2d> makeMesh(QSize& /*dims*/) const {return {};}

    NAME_ICON_FUNCTIONS("X", ":/profiles/ProfileX.png") // use some default icon

    static vec2d getAbsMin(const Box2D& b);
    static vec2d getAbsMax(const Box2D& b);
};



#include "kernel/node/TFactory.h"

class ProfileFactory: public TFactory
{
public:
    virtual ProfileRT* create() const = 0;
};

Q_DECLARE_INTERFACE(ProfileFactory, "tonatiuh.ProfileFactory")


template<class T>
class ProfileFactoryT: public ProfileFactory
{
public:
    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    void init() const {T::initClass();}
    T* create() const {return new T;}
};
