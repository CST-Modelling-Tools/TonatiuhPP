#pragma once

#include "kernel/scene/TAbstract.h"
#include <Inventor/nodes/SoSubNode.h>


class TONATIUH_KERNEL Air: public SoNode
{
    SO_NODE_ABSTRACT_HEADER(Air);

public:
    static void initClass();

    virtual double transmission(double /*distance*/) const = 0;

    NAME_ICON_FUNCTIONS("X", ":/images/AirAbstract.png")

protected:
    Air() {}
    ~Air() {}
};



#include "kernel/scene/TFactory.h"

class AirFactory: public TFactory
{
public:
    virtual Air* create() const = 0;
};

Q_DECLARE_INTERFACE(AirFactory, "tonatiuh.AirFactory")


template<class T>
class AirFactoryT: public AirFactory
{
public:
    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    void init() const {T::initClass();}
    T* create() const {return new T;}
};
