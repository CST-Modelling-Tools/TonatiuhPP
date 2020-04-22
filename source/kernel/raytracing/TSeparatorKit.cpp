#include <QString>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

#include "TDefaultTracker.h"
#include "TSeparatorKit.h"

SO_KIT_SOURCE(TSeparatorKit);

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

    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(tracker, SoNode, TDefaultTracker, TRUE, this, "", TRUE);
    SO_KIT_INIT_INSTANCE();

    SoTransform* transform = new SoTransform;
    setPart("transform", transform);

}

/**
 * TSeparatorKit destructor.
 */
TSeparatorKit::~TSeparatorKit()
{

}
/**
 * Returns a pointer to the node part with \a partname.
 */
SoNode* TSeparatorKit::getPart( const SbName &partname, SbBool makeifneeded )
{
     return SoSeparatorKit::getPart( partname, makeifneeded );
}

/**
 * Set \a from node as \a partname node.
 */
SbBool  TSeparatorKit::setPart(const SbName& partname, SoNode* from )
{
    if( partname == "tracker" )
    {

        SoTransform* parentTransform = static_cast< SoTransform* > ( getPart("transform", true ) );
        if( !parentTransform ) return false;

        if( !from )
        {
            parentTransform->translation.disconnect();
            parentTransform->rotation.disconnect();
            parentTransform->scaleFactor.disconnect();
            parentTransform->scaleOrientation.disconnect();
            parentTransform->center.disconnect();
        }
        else
        {
            TTracker* trackerNode = static_cast< TTracker* >( from );
            trackerNode->ConnectParentTranform(parentTransform);
        }
    }

    return SoSeparatorKit::setPart( partname, from );
}

/**
 * Returns the icon that represents TSeparatorKit nodes.
 */
QString TSeparatorKit::getIcon()
{
    return QLatin1String(":/icons/separatorKit.png");

}
