#include "Photon.h"


//Photon::Photon():
//    id(-1),
//    pos(vec3d()),
//    surface(0),
//    isFront(false),
//    isAbsorbed(false)
//{

//}

Photon::Photon(int id, const vec3d& pos, InstanceNode* surface, bool isFront, bool isAbsorbed):
    id(id),
    pos(pos),
    surface(surface),
    isFront(isFront),
    isAbsorbed(isAbsorbed)
{

}
