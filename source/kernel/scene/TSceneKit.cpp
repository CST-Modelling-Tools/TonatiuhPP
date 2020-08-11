#include "TSceneKit.h"

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoGroup.h>

#include "libraries/math/gcf.h"
#include "libraries/math/3D/Transform.h"
#include "libraries/math/3D/vec3d.h"

#include "kernel/trackers/Tracker.h"
#include "kernel/air/AirTransmission.h"
#include "kernel/sun/SunPillbox.h"
#include "kernel/sun/SunKit.h"
#include "kernel/sun/SunPosition.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include "WorldKit.h"
#include "LocationNode.h"

SO_KIT_SOURCE(TSceneKit)


void TSceneKit::initClass()
{
    SO_KIT_INIT_CLASS(TSceneKit, SoSceneKit, "SceneKit");

    WorldKit::initClass();
    SunKit::initClass();

    TSeparatorKit::initClass();
    TShapeKit::initClass();
}

TSceneKit::TSceneKit()
{
    SO_KIT_CONSTRUCTOR(TSceneKit);

    SO_NODE_ADD_FIELD(version, ("") );
    version.setValue("2020");

    SO_KIT_ADD_CATALOG_ENTRY(world, WorldKit, FALSE, this, "", TRUE);

    SO_KIT_ADD_CATALOG_ENTRY(group, TSeparatorKit, FALSE, topSeparator, "", TRUE); // topsep important
//    SO_KIT_CHANGE_ENTRY_TYPE(childList, SoGroup, SoGroup);

    SO_KIT_INIT_INSTANCE();

    setSearchingChildren(true); // for lightList[0]

    SunKit* sunKit = new SunKit;
    setPart("lightList[0]", sunKit);

//    LocationNode* loc = (LocationNode*) getPart("world.location", true);
//    loc->name = "unknown";

    TSeparatorKit* nodeLayout = (TSeparatorKit*) getPart("group", true);
    nodeLayout->setName("Layout");
}

TSeparatorKit* TSceneKit::getLayout()
{
    return (TSeparatorKit*) getPart("group", false);
}

void TSceneKit::updateTrackers()
{
    SunPosition* sp = (SunPosition*) getPart("world.sun.position", false);
    vec3d vSun = sp->getSunVector();
    updateTrackers(getLayout(), Transform::Identity, vSun);
}

/*!
 * Updates all trackers transform for the current sun angles.
 */
void TSceneKit::updateTrackers(TSeparatorKit* parent, Transform toGlobal, const vec3d& vSun)
{
    SoTransform* tParent = (SoTransform*) parent->getPart("transform", true);
    Transform t = toGlobal*tgf::makeTransform(tParent);

    if (Tracker* tracker = (Tracker*) parent->getPart("tracker", false))
    {
        tracker->update(parent, t, vSun);
    }
    else if (SoGroup* nodes = (SoGroup*) parent->getPart("group", false))
    {
        for (int n = 0; n < nodes->getNumChildren(); ++n)
        {
            TSeparatorKit* child = dynamic_cast<TSeparatorKit*>(nodes->getChild(n));
            if (child) updateTrackers(child, t, vSun);
        }
    }
}
