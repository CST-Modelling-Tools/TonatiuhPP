#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFVec3f.h>

#include <QString>

#include "libraries/geometry/gc.h"

#include "TDefaultTracker.h"

SO_NODEENGINE_SOURCE(TDefaultTracker)

/**
 * Sets up initialization for data common to all instances of this class, like submitting necessary information to the Coin type system.
 *
 */
void TDefaultTracker::initClass()
{
    SO_NODEENGINE_INIT_CLASS(TDefaultTracker, TrackerAbstract, "TrackerAbstract");
}

/**
 * TDefaultTracker constructor
 */
TDefaultTracker::TDefaultTracker()
{
    SO_NODEENGINE_CONSTRUCTOR(TDefaultTracker);

    //SO_NODE_ADD_FIELD( m_azimuth, ( gc::Pi ) );
    //SO_NODE_ADD_FIELD( m_zenith, ( 0.0 ) );

    //ConstructEngineOutput();
    // Define input fields and their default values
    SO_NODEENGINE_ADD_OUTPUT(outputTranslation, SoSFVec3f);
    SO_NODEENGINE_ADD_OUTPUT(outputRotation, SoSFRotation);
    SO_NODEENGINE_ADD_OUTPUT(outputScaleFactor, SoSFVec3f);
    SO_NODEENGINE_ADD_OUTPUT(outputScaleOrientation, SoSFRotation);
    SO_NODEENGINE_ADD_OUTPUT(outputCenter, SoSFVec3f);
}

/**
 * TDefaultTracker destructor
 */
TDefaultTracker::~TDefaultTracker()
{

}

/**
 * Computes the rotation ouput.
 */
void TDefaultTracker::evaluate()
{
    SetEngineOutputIdentity();
}
