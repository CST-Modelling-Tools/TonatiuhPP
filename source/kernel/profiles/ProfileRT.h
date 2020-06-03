#pragma once

#include <QVector>
#include <QSize>

#include <Inventor/nodes/SoSubNode.h>

#include "kernel/scene/TAbstract.h"
#include "libraries/geometry/Vector2D.h"
#include "libraries/geometry/BoundingBox.h"

struct DifferentialGeometry;
class Random;
class Ray;


class TONATIUH_KERNEL ProfileRT: public SoNode
{
    SO_NODE_ABSTRACT_HEADER(ProfileRT);

public:
    static void initClass();

    virtual BoundingBox getBox() const {return BoundingBox::UnitCube;}
    virtual bool isInside(double /*x*/, double /*y*/) const {return true;}
    virtual QVector<Vector2D> makeMesh(const QSize& /*dims*/) const {return {};}

    NAME_ICON_FUNCTIONS("X", ":/MaterialX.png") // use some default icon

protected:
    ProfileRT() {}
    ~ProfileRT() {}
};



#include "kernel/scene/TFactory.h"

class ApertureFactory: public TFactory
{
public:
    virtual ProfileRT* create() const = 0;
};

Q_DECLARE_INTERFACE(ApertureFactory, "tonatiuh.ApertureFactory")


template<class T>
class ApertureFactoryT: public ApertureFactory
{
public:
    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    void init() const {T::initClass();}
    T* create() const {return new T;}
};
