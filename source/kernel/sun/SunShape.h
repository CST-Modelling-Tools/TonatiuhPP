#pragma once

#include "kernel/scene/TAbstract.h"
#include "kernel/random/Random.h"
#include "libraries/math/3D/vec3d.h"

#include <Inventor/nodes/SoSubNode.h>



class TONATIUH_KERNEL SunShape: public SoNode
{
    SO_NODE_ABSTRACT_HEADER(SunShape);

public:
    static void initClass();

    virtual vec3d generateRay(Random& rand) const = 0;
    virtual double getThetaMax() const = 0;

    NAME_ICON_FUNCTIONS("X", ":/SunX.png")

protected:
    SunShape() {}
    ~SunShape() {}
};



#include "kernel/scene/TFactory.h"

class SunFactory: public TFactory
{
public:
    virtual SunShape* create() const = 0;
};

Q_DECLARE_INTERFACE(SunFactory, "tonatiuh.SunFactory")


template<class T>
class SunFactoryT: public SunFactory
{
public:
    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    void init() const {T::initClass();}
    T* create() const {return new T;}
};
