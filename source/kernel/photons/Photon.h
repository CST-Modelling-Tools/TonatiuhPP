#pragma once

#include "kernel/TonatiuhKernel.h"

#include "kernel/run/InstanceNode.h"
#include "libraries/math/3D/vec3d.h"


struct TONATIUH_KERNEL Photon
{
    Photon();
    Photon(double id, const vec3d& pos, InstanceNode* surface, int isFront, int isAbsorbed = 0);

    // number of point along ray path, from 0
    double id; //? int

    // point along ray path
    vec3d pos;

    // intersected surface
    // light source for start
    // 0 for air
    InstanceNode* intersectedSurface;

    // front side of intersected surface
    // front (true) for light source
    // back (false) for air
    int isFront;

    // 1 for reflected?
    // 0 otherwise
    int isAbsorbed;
};
