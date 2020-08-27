#include "TrackerTarget.h"


SO_NODE_SOURCE(TrackerTarget)


void TrackerTarget::initClass()
{
    SO_NODE_INIT_CLASS(TrackerTarget, SoNode, "Node");
}

TrackerTarget::TrackerTarget()
{
    SO_NODE_CONSTRUCTOR(TrackerTarget);

    SO_NODE_DEFINE_ENUM_VALUE(AimingFrame, global);
    SO_NODE_DEFINE_ENUM_VALUE(AimingFrame, facets);
    SO_NODE_SET_SF_ENUM_TYPE(aimingFrame, AimingFrame);
    SO_NODE_ADD_FIELD( aimingFrame, (global) );
    SO_NODE_ADD_FIELD( aimingPoint, (0.f, 0.f, 100.f) );
}

TrackerTarget::~TrackerTarget()
{

}
