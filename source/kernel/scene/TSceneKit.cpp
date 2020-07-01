#include "TSceneKit.h"

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoGroup.h>

#include "libraries/math/gcf.h"
#include "libraries/math/Transform.h"
#include "libraries/math/Vector3D.h"

#include "kernel/air/AirVacuum.h"
#include "kernel/trackers/Tracker.h"
#include "kernel/air/Air.h"
#include "kernel/sun/SunPillbox.h"
#include "kernel/sun/SunKit.h"
#include "kernel/scene/TSeparatorKit.h"


SO_KIT_SOURCE(TSceneKit)

void TSceneKit::initClass()
{
    SO_KIT_INIT_CLASS(TSceneKit, SoSceneKit, "SceneKit");
}

TSceneKit::TSceneKit()
{
    SO_KIT_CONSTRUCTOR(TSceneKit);
//    SO_KIT_CHANGE_ENTRY_TYPE(childList, SoGroup, SoGroup);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(group, SoGroup, SoGroup, TRUE, topSeparator, "", TRUE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(air, Air, AirVacuum, TRUE, topSeparator, "", TRUE);
    SO_KIT_INIT_INSTANCE();

    SunKit* sunKit = new SunKit;
    setPart("lightList[0]", sunKit);
    setPart("air", new AirVacuum);
}

void TSceneKit::updateTrackers()
{
    SoGroup* nodes = (SoGroup*) getPart("group", true);
    if (!nodes->getNumChildren()) return;
    auto node = (TSeparatorKit*) nodes->getChild(0); // Layout
    if (!node) return;

    SunKit* sunKit = (SunKit*) getPart("lightList[0]", false);
    double az = sunKit->azimuth.getValue();
    double el = sunKit->elevation.getValue();

    Vector3D vSun(
        sin(az)*cos(el),
        cos(az)*cos(el),
        sin(el)
    );

    updateTrackers(node, Transform::Identity, vSun);
}

/*!
 * Updates all trackers transform for the current sun angles.
 */
void TSceneKit::updateTrackers(TSeparatorKit* parent, Transform toGlobal, const Vector3D& vSun)
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
