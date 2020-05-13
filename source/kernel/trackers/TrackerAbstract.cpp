#include "TrackerAbstract.h"


SO_NODE_ABSTRACT_SOURCE(TrackerAbstract)


void TrackerAbstract::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(TrackerAbstract, SoNode, "Node");
}

void TrackerAbstract::Evaluate(SoBaseKit* parent, const Transform& transform, const Vector3D& vSun)
{
    Q_UNUSED(parent)
    Q_UNUSED(transform) // transformOtW
    Q_UNUSED(vSun)
}
