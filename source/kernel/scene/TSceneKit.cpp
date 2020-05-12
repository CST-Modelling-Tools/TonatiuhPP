#include "TSceneKit.h"

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>

#include "libraries/geometry/gc.h"
#include "libraries/geometry/Transform.h"
#include "libraries/geometry/Vector3D.h"

#include "kernel/air/AirVacuum.h"
#include "TSeparatorKit.h"
#include "kernel/trackers/TrackerAbstract.h"
#include "kernel/air/AirAbstract.h"

SO_KIT_SOURCE(TSceneKit)

/**
 * Does initialization common for all objects of the TSceneKit class.
 * This includes setting up the type system, among other things.
 */
void TSceneKit::initClass()
{
    SO_KIT_INIT_CLASS(TSceneKit, SoSceneKit, "SceneKit");
}

/**
 * Creates a new TSceneKit node.
 */

#include "sun/SunPillbox.h"
#include "sun/TLightKit.h"

TSceneKit::TSceneKit()
{
    SO_KIT_CONSTRUCTOR(TSceneKit);

    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(transmissivity, AirAbstract, AirVacuum, TRUE, topSeparator, "", TRUE);

    SO_NODE_ADD_FIELD( azimuth, (0.) );
    SO_NODE_ADD_FIELD( elevation, (90.) );

    SO_KIT_INIT_INSTANCE();

    TLightKit* lightKit = new TLightKit;
    lightKit->setPart("tsunshape", new SunPillbox);
    setPart("lightList[0]", lightKit);

    setPart("transmissivity", new AirVacuum);
}

/*!
 * Returns the path from the scene node to the node in \a action.
 */
SoPath* TSceneKit::GetSoPath(SoSearchAction* action)
{
    TSeparatorKit* sunNode = static_cast<TSeparatorKit*> (getPart("childList[0]", false) );
    if (!sunNode) return NULL;

    TSeparatorKit* rootNode = static_cast<TSeparatorKit*> (sunNode->getPart("childList[0]", false) );
    if (!rootNode) return NULL;

    action->setInterest(SoSearchAction::FIRST);
    action->apply(rootNode);
    SoPath* nodePath = action->getPath();
    return nodePath;
}

void TSceneKit::updateTrackers(double az, double el)
{
    azimuth = az;
    elevation = el;

    Vector3D vSunW(
        sin(az)*cos(el),
        cos(az)*cos(el),
        sin(el)
    ); // to sun

    Transform transformOtW(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );

    auto nodes = static_cast<SoNodeKitListPart*>(getPart("childList", true));
    if (nodes->getNumChildren() < 1) return;

    auto node = static_cast<SoBaseKit*>(nodes->getChild(0));     //SunNode
    if (!node) return;

    nodes = static_cast<SoNodeKitListPart*>(node->getPart("childList", true));

    for (int n = 0; n < nodes->getNumChildren(); ++n)
    {
        node = static_cast<SoBaseKit*>(nodes->getChild(n));
        UpdateTrackersTransform(node, vSunW, transformOtW);
    }
}

/*!
 * Updates all trackers transform for the current sun angles.
 */
void TSceneKit::UpdateTrackersTransform(SoBaseKit* parent, const Vector3D& vSunW, Transform transformOtW)
{
    if (!parent) return;

    if (auto tracker = (TrackerAbstract*) parent->getPart("tracker", false))
    {
        tracker->Evaluate(parent, transformOtW, vSunW);
    }
    else if (parent->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
    {
        SoTransform* nodeTransform = static_cast<SoTransform*>(parent->getPart("transform", true));
        Transform nodeOTW = transformOtW*tgf::TransformFromSoTransform(nodeTransform);

        auto nodes = (SoNodeKitListPart*) parent->getPart("childList", false);
        if (!nodes) return;
        for (int n = 0; n < nodes->getNumChildren(); ++n)
        {
            SoBaseKit* child = static_cast<SoBaseKit*>(nodes->getChild(n));
            if (child) UpdateTrackersTransform(child, vSunW, nodeOTW);
        }
    }
}
