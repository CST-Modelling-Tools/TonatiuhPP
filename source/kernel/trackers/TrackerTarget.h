#pragma once

#include "kernel/scene/TNode.h"


class TONATIUH_KERNEL TrackerTarget: public SoNode
{
    SO_NODE_HEADER(TrackerTarget);

public:
    static void initClass();
    TrackerTarget();
    ~TrackerTarget();

    enum AimingFrame {
        global = 0,
        facets = 1
    };
    SoSFEnum aimingFrame;
    SoSFVec3f aimingPoint;
};

