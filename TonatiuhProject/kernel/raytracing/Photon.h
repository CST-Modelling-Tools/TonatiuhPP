#pragma once

#include "kernel/gui/InstanceNode.h"
#include "Point3D.h"

struct Photon
{
    Photon();
    Photon(const Photon& photon);
    Photon(Point3D pos, int side, double id = 0, InstanceNode* intersectedSurface = 0, int absorbedPhoton = 0);

    double id;
    Point3D pos;
    int side;
    InstanceNode* intersectedSurface;
    int isAbsorbed;
};


