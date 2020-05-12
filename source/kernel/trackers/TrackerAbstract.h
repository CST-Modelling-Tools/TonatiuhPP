#pragma once

#include "kernel/scene/TAbstract.h"

#include <Inventor/nodes/SoSubNode.h>

struct Vector3D;
class Transform;


class TONATIUH_KERNEL TrackerAbstract: public SoNode
{
    SO_NODE_ABSTRACT_HEADER(TrackerAbstract);

public:
    static void initClass();

    virtual void Evaluate(SoNode* parent, const Transform& transform, const Vector3D& vSun);

    NAME_ICON_FUNCTIONS("X", ":/TrackerX.png")

protected:
    TrackerAbstract() {}
    ~TrackerAbstract() {}
};
