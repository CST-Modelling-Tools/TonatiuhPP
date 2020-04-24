#pragma once

#include "kernel/gui/InstanceNode.h"
#include "libraries/geometry/Point3D.h"

struct Photon
{
    Photon();
    Photon(const Point3D& pos, int side, double id = 0, InstanceNode* intersectedSurface = 0, int absorbedPhoton = 0);

    // number of point along ray path, from 0
    double id;

    // point along ray path
    Point3D pos;

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


