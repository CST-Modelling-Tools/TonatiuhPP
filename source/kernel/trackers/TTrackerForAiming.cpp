#include "TTrackerForAiming.h"
//#include "kernel/scene/TSceneKit.h"
//#include "libraries/geometry/Transform.h"
//#include "kernel/trf.h"


SO_NODEENGINE_ABSTRACT_SOURCE(TTrackerForAiming)


void TTrackerForAiming::initClass()
{
    SO_NODEENGINE_INIT_ABSTRACT_CLASS(TTrackerForAiming, TrackerAbstract, "TrackerAbstract");
}

TTrackerForAiming::TTrackerForAiming()
{
    /*SO_NODEENGINE_CONSTRUCTOR( TTrackerForAiming );

    SO_NODE_DEFINE_ENUM_VALUE( AimingPointType, Absolute );
    SO_NODE_DEFINE_ENUM_VALUE( AimingPointType, Relative );
    SO_NODE_SET_SF_ENUM_TYPE( typeOfAimingPoint, AimingPointType );
    SO_NODE_ADD_FIELD( typeOfAimingPoint, (Absolute) );
    SoFieldSensor* m_infoDisplayed = new SoFieldSensor( updateTypeOfAimingPoint, this );
    m_infoDisplayed->attach( &typeOfAimingPoint );*/
}

void TTrackerForAiming::SetAimingPointRelativity(bool relative)
{
    if ((typeOfAimingPoint.getValue() == AimingPointType::Relative) == relative)
        return;

    if (relative)
        typeOfAimingPoint.setValue(AimingPointType::Relative);
    else
        typeOfAimingPoint.setValue(AimingPointType::Absolute);
}
