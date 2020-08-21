#include "TSeparatorKit.h"

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include "kernel/trackers/Tracker.h"

SO_KIT_SOURCE(TSeparatorKit)


void TSeparatorKit::initClass()
{
    SO_KIT_INIT_CLASS(TSeparatorKit, SoBaseKit, "BaseKit");

    Tracker::initClass();
}

TSeparatorKit::TSeparatorKit()
{
    SO_KIT_CONSTRUCTOR(TSeparatorKit);
    isBuiltIn = TRUE; // do not save the list of fields

    SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, TRUE, this, "", FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(transform, SoTransform, FALSE, topSeparator, "", TRUE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(tracker, SoNode, Tracker, TRUE, this, "", TRUE);
//    SO_KIT_CHANGE_ENTRY_TYPE(childList, SoGroup, SoGroup); // without node lists
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(group, SoGroup, SoGroup, TRUE, topSeparator, "", TRUE); // temp
    SO_KIT_INIT_INSTANCE();
}


// use bounding box in world coordinates
void TSeparatorKit::getBoundingBox(SoGetBoundingBoxAction* action)
{
    SoBaseKit::getBoundingBox(action);
    SbXfBox3f& box = action->getXfBoundingBox();
    box = box.project(); // not optimal
//    box.setTransform(SbMatrix::identity());
}
