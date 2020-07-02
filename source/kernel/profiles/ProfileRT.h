#pragma once

#include <QVector>
#include <QSize>

#include <Inventor/nodes/SoSubNode.h>

#include "kernel/scene/TAbstract.h"
#include "libraries/math/2D/vec2d.h"
#include "libraries/math/2D/Box2D.h"

struct DifferentialGeometry;
class Random;
class Ray;


class TONATIUH_KERNEL ProfileRT: public SoNode
{
    SO_NODE_ABSTRACT_HEADER(ProfileRT);

public:
    static void initClass();

    virtual Box2D getBox() const {return Box2D::UnitCentered;}
    virtual bool isInside(double /*x*/, double /*y*/) const {return true;}
    virtual QVector<vec2d> makeMesh(QSize& /*dims*/) const {return {};}

    NAME_ICON_FUNCTIONS("X", ":/MaterialX.png") // use some default icon

    static vec2d getAbsMin(const Box2D& b);
    static vec2d getAbsMax(const Box2D& b);

protected:
    ProfileRT() {}
    ~ProfileRT() {}
};



#include "kernel/scene/TFactory.h"

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
