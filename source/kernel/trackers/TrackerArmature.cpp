#include "TrackerArmature.h"


SO_NODE_ABSTRACT_SOURCE(TrackerArmature)


void TrackerArmature::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(TrackerArmature, TNode, "TNode");
}

void TrackerArmature::update(TSeparatorKit* parent, const Transform& toGlobal, const vec3d& vSun, TrackerTarget* target)
{
    Q_UNUSED(parent)
    Q_UNUSED(toGlobal)
    Q_UNUSED(vSun)
    Q_UNUSED(target)
}
