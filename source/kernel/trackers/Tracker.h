#pragma once

#include "kernel/scene/TNode.h"

#include <Inventor/nodekits/SoBaseKit.h>

struct vec3d;
class Transform;
class TSeparatorKit;


class TONATIUH_KERNEL Tracker: public TNode
{
    SO_NODE_ABSTRACT_HEADER(Tracker);

public:
    static void initClass();

    virtual void update(TSeparatorKit* parent, const Transform& toGlobal, const vec3d& vSun);

    NAME_ICON_FUNCTIONS("X", ":/TrackerX.png")
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
