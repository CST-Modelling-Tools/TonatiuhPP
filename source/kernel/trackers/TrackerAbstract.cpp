#include "TrackerAbstract.h"


SO_NODE_ABSTRACT_SOURCE(TrackerAbstract)


void TrackerAbstract::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(TrackerAbstract, SoNode, "Node");
}

void TrackerAbstract::update(SoBaseKit* parent, const Transform& toGlobal, const Vector3D& vSun)
{
    Q_UNUSED(parent)
    Q_UNUSED(toGlobal)
    Q_UNUSED(vSun)
}
