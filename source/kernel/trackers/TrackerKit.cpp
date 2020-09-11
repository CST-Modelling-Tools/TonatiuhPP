#include "TrackerKit.h"

#include "TrackerArmature2A.h"
#include "TrackerTarget.h"

SO_KIT_SOURCE(TrackerKit)


void TrackerKit::initClass()
{
    SO_KIT_INIT_CLASS(TrackerKit, SoBaseKit, "BaseKit");

    TrackerArmature::initClass();
    TrackerTarget::initClass();
}

TrackerKit::TrackerKit()
{
    SO_KIT_CONSTRUCTOR(TrackerKit);
    isBuiltIn = TRUE;

    SO_NODE_ADD_FIELD( armature, (0) );
    SO_NODE_ADD_FIELD( target, (0) );

    SO_KIT_INIT_INSTANCE();

    armature = new TrackerArmature2A;
    target = new TrackerTarget;
}

void TrackerKit::update(TSeparatorKit* parent, const Transform& toGlobal, const vec3d& vSun)
{
    TrackerArmature* ta = (TrackerArmature*) armature.getValue();
    TrackerTarget* tt = (TrackerTarget*) target.getValue();
    ta->update(parent, toGlobal, vSun, tt);
}
