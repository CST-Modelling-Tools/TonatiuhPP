#pragma once

#include "kernel/TonatiuhKernel.h"

#include "kernel/run/InstanceNode.h"
#include "libraries/math/Vector3D.h"


struct TONATIUH_KERNEL Photon
{
    Photon();
    Photon(const Vector3D& pos, int side, double id = 0, InstanceNode* intersectedSurface = 0, int absorbedPhoton = 0);

    // number of point along ray path, from 0
    double id;

    // point along ray path
    Vector3D pos;

    // front side of intersected surface
    // front (true) for light source
    // back (false) for air
    int side;

    // intersected surface
    // light source for start
    // 0 for air
    InstanceNode* intersectedSurface;

    // 1 for reflected?
    // 0 otherwise
    int isAbsorbed;
};


