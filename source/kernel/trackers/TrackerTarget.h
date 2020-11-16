#pragma once

#include "kernel/node/TNode.h"


class TONATIUH_KERNEL TrackerTarget: public SoNode
{
    SO_NODE_HEADER(TrackerTarget);

public:
    static void initClass();
    TrackerTarget();
    ~TrackerTarget();

    enum AimingType {
//        angles,
        local,
        global
    };

    SoSFEnum aimingType;
    SoSFVec3f aimingPoint;
    SoSFVec2f angles;
};

