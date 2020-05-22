#pragma once

#include "kernel/scene/TAbstract.h"

#include "kernel/TonatiuhTypes.h"
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodekits/SoBaseKit.h>

struct Vector3D;
class Transform;


class TONATIUH_KERNEL Tracker: public SoNode
{
    SO_NODE_ABSTRACT_HEADER(Tracker);

public:
    static void initClass();

    virtual void update(SoBaseKit* parent, const Transform& toGlobal, const Vector3D& vSun);

    NAME_ICON_FUNCTIONS("X", ":/TrackerX.png")

protected:
    Tracker() {}
    ~Tracker() {}
};



#include "kernel/scene/TFactory.h"

class TrackerFactory: public TFactory
{
public:
    virtual Tracker* create() const = 0;
};

Q_DECLARE_INTERFACE(TrackerFactory, "tonatiuh.TrackerFactory")


template<class T>
class TrackerFactoryT: public TrackerFactory
{
public:
    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    void init() const {T::initClass();}
    T* create() const {return new T;}
};
