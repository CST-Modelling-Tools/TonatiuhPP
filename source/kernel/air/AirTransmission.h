#pragma once

#include "kernel/node/TNode.h"


class TONATIUH_KERNEL AirTransmission: public TNode
{
    SO_NODE_ABSTRACT_HEADER(AirTransmission);

public:
    static void initClass();

    virtual double transmission(double distance) const = 0;

    NAME_ICON_FUNCTIONS("Air", ":/images/AirX.png")
};



#include "kernel/node/TFactory.h"

class AirFactory: public TFactory
{
public:
    virtual AirTransmission* create() const = 0;
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
