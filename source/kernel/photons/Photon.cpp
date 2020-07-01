#include "Photon.h"


Photon::Photon():
    id(-1),
    pos(vec3d()),
    side(-1),
    intersectedSurface(0),
    isAbsorbed(-1)
{

}

Photon::Photon(const vec3d& pos, int side, double id, InstanceNode* intersectedSurface, int absorbedPhoton):
    id(id),
    pos(pos),
    side(side),
    intersectedSurface(intersectedSurface),
    isAbsorbed(absorbedPhoton)
{

}

