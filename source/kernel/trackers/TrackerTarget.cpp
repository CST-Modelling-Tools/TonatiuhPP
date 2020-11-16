#include "TrackerTarget.h"


SO_NODE_SOURCE(TrackerTarget)


void TrackerTarget::initClass()
{
    SO_NODE_INIT_CLASS(TrackerTarget, SoNode, "Node");
}

TrackerTarget::TrackerTarget()
{
    SO_NODE_CONSTRUCTOR(TrackerTarget);
    isBuiltIn = TRUE;

//    SO_NODE_DEFINE_ENUM_VALUE(AimingType, angles);
    SO_NODE_DEFINE_ENUM_VALUE(AimingType, local);
    SO_NODE_DEFINE_ENUM_VALUE(AimingType, global);
    SO_NODE_SET_SF_ENUM_TYPE(aimingType, AimingType);
    SO_NODE_ADD_FIELD( aimingType, (global) );
    SO_NODE_ADD_FIELD( aimingPoint, (0.f, 0.f, 100.f) );
    SO_NODE_ADD_FIELD( angles, (0.f, 0.f) );
}

TrackerTarget::~TrackerTarget()
{

}
