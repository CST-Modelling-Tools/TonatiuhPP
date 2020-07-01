#include "Tracker.h"


SO_NODE_ABSTRACT_SOURCE(Tracker)


void Tracker::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(Tracker, SoNode, "Node");
}

void Tracker::update(TSeparatorKit* parent, const Transform& toGlobal, const vec3d& vSun)
{
    Q_UNUSED(parent)
    Q_UNUSED(toGlobal)
    Q_UNUSED(vSun)
}
