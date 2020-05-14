#include "Photon.h"


Photon::Photon():
    id(-1),
    pos(Vector3D()),
    side(-1),
    intersectedSurface(0),
    isAbsorbed(-1)
{

}

Photon::Photon(const Vector3D& pos, int side, double id, InstanceNode* intersectedSurface, int absorbedPhoton):
    id(id),
    pos(pos),
    side(side),
    intersectedSurface(intersectedSurface),
    isAbsorbed(absorbedPhoton)
{

}

