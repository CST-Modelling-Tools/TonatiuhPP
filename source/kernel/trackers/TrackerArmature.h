#pragma once

#include "kernel/node/TNode.h"

#include <Inventor/nodekits/SoBaseKit.h>

class TSeparatorKit;
class Transform;
class SoShapeKit;
struct vec3d;
class TrackerTarget;


class TONATIUH_KERNEL TrackerArmature: public TNode
{
    SO_NODE_ABSTRACT_HEADER(TrackerArmature);

public:
    static void initClass();

    virtual void update(TSeparatorKit* parent, const Transform& toGlobal,
                        const vec3d& vSun, TrackerTarget* target);

    virtual void updateShape(TSeparatorKit* parent, SoShapeKit* shape, TrackerTarget* target);

    NAME_ICON_FUNCTIONS("X", ":/TrackerX.png")
};



#include "kernel/node/TFactory.h"

class TrackerFactory: public TFactory
{
public:
    virtual TrackerArmature* create() const = 0;
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
