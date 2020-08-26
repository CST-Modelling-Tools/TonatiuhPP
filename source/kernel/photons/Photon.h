#pragma once

#include "kernel/TonatiuhKernel.h"

#include "kernel/run/InstanceNode.h"
#include "libraries/math/3D/vec3d.h"


struct TONATIUH_KERNEL Photon
{
    Photon() {}
    Photon(int id, const vec3d& pos, InstanceNode* surface, bool isFront, bool isAbsorbed = false);

    // number of point along ray path, from 0
    int id;

    // point along ray path
    vec3d pos;

    // intersected surface
    // light source for start
    // 0 for air
    InstanceNode* surface;

    // front side of intersected surface
    // front (true) for light source
    // back (false) for air
    bool isFront;

    // 1 for reflected?
    // 0 otherwise
    bool isAbsorbed;
};

// 4 + 4 + 2 + 3*8 = 34 bytes/photon
