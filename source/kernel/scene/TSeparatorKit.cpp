#include "TSeparatorKit.h"

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include "kernel/trackers/TrackerKit.h"



SO_KIT_SOURCE(TSeparatorKit)


void TSeparatorKit::initClass()
{
    SO_KIT_INIT_CLASS(TSeparatorKit, SoBaseKit, "BaseKit");

    TrackerKit::initClass();
    TTransform::initClass();
}

TSeparatorKit::TSeparatorKit()
{
    SO_KIT_CONSTRUCTOR(TSeparatorKit);
    isBuiltIn = TRUE; // do not save the list of fields

    SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, FALSE, this, "", FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(transform, TTransform, FALSE, topSeparator, "", TRUE);

    SO_KIT_ADD_CATALOG_ENTRY(components, SoGroup, TRUE, topSeparator, "", TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(group, SoGroup, TRUE, topSeparator, "", TRUE);

    SO_KIT_INIT_INSTANCE();
}

//// finish!
//TSeparatorKit* TSeparatorKit::copy(SbBool copyConnections) const
//{
//    TSeparatorKit* kit = dynamic_cast<TSeparatorKit*>(SoBaseKit::copy(copyConnections));
////    kit->m_shapeKit = new SoShapeKit;
////    SoGroup* g = (SoGroup*) kit->topSeparator.getValue();
////    g->removeAllChildren();
////    g->addChild(kit->m_shapeKit);
////    kit->m_shapeKit->setPart("material", kit->material.getValue());

//    return kit;
//}

// use bounding box in world coordinates ?
void TSeparatorKit::getBoundingBox(SoGetBoundingBoxAction* action)
{
    SoBaseKit::getBoundingBox(action);
    SbXfBox3f& box = action->getXfBoundingBox();
    box = box.project(); // not optimal
    //    box.setTransform(SbMatrix::identity());
}

//void TSeparatorKit::setCulling(bool on)
//{
//    SoSeparator* sep = (SoSeparator*) topSeparator.getValue();
//    sep->renderCulling = on ? SoSeparator::ON : SoSeparator::OFF;
//}


//    SbViewportRegion vpr;// = m_graphicView[m_focusView]->getViewportRegion();
//    SbViewportRegion vr;
//    SoGetBoundingBoxAction action(vr);
//    action.apply(node);
//    node->getNode()->getBoundingBox(&action);
//    SbBox3f box = action.getBoundingBox();

//    SbXfBox3f box = action.getXfBoundingBox();
//    if (box.isEmpty()) return;
//    box.getTransform().multVecMatrix(box.getCenter(), vG);

//    SO_KIT_CHANGE_ENTRY_TYPE(childList, SoGroup, SoGroup); // without node lists
