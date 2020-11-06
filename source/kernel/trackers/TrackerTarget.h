#pragma once

#include "kernel/node/TNode.h"


class TONATIUH_KERNEL TrackerTarget: public SoNode
{
    SO_NODE_HEADER(TrackerTarget);

public:
    static void initClass();
    TrackerTarget();
    ~TrackerTarget();

    enum AimingFrame {
        facets,
        global
    };

    SoSFEnum aimingFrame;
    SoSFVec3f aimingPoint;
};

