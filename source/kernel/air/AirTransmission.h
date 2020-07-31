#pragma once

#include "kernel/scene/TAbstract.h"
#include <Inventor/nodes/SoSubNode.h>


class TONATIUH_KERNEL AirTransmission: public SoNode
{
    SO_NODE_HEADER(AirTransmission);

public:
    static void initClass();
    AirTransmission();

    virtual double transmission(double /*distance*/) const {return 1.;}

    NAME_ICON_FUNCTIONS("Vacuum", ":/images/AirVacuum.png")
};



#include "kernel/scene/TFactory.h"

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
