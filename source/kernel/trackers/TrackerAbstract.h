#pragma once

#include "kernel/scene/TAbstract.h"

#include "kernel/TonatiuhTypes.h"
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodekits/SoBaseKit.h>

struct Vector3D;
class Transform;


class TONATIUH_KERNEL TrackerAbstract: public SoNode
{
    SO_NODE_ABSTRACT_HEADER(TrackerAbstract);

public:
    static void initClass();

    virtual void update(SoBaseKit* parent, const Transform& toGlobal, const Vector3D& vSun);

    NAME_ICON_FUNCTIONS("X", ":/TrackerX.png")

protected:
    TrackerAbstract() {}
    ~TrackerAbstract() {}
};
