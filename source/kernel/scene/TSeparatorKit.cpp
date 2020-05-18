#include "TSeparatorKit.h"

#include <QString>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

#include "kernel/trackers/TrackerAbstract.h"

SO_KIT_SOURCE(TSeparatorKit)

/**
 * Does initialization common for all objects of the TSeparatorKit class.
 * This includes setting up the type system, among other things.
 */
void TSeparatorKit::initClass()
{
    SO_KIT_INIT_CLASS(TSeparatorKit, SoSeparatorKit, "SeparatorKit");
}

/**
 * Creates a new TSeparatorKit node.
 */
TSeparatorKit::TSeparatorKit()
{
    SO_KIT_CONSTRUCTOR(TSeparatorKit);

    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(tracker, SoNode, TrackerAbstract, TRUE, this, "", TRUE);
    SO_KIT_INIT_INSTANCE();

    SoTransform* transform = new SoTransform;
    setPart("transform", transform);
}

#include <Inventor/actions/SoGetBoundingBoxAction.h>
// use bounding box in world coordinates
void TSeparatorKit::getBoundingBox(SoGetBoundingBoxAction* action)
{
    SoSeparatorKit::getBoundingBox(action);
    SbXfBox3f& box = action->getXfBoundingBox();
    box = box.project();
//    box.setTransform(SbMatrix::identity());
}
