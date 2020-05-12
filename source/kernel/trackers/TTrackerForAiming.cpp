#include "TTrackerForAiming.h"


SO_NODE_ABSTRACT_SOURCE(TTrackerForAiming)


void TTrackerForAiming::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(TTrackerForAiming, TrackerAbstract, "TrackerAbstract");
}

TTrackerForAiming::TTrackerForAiming()
{
//    SO_NODE_CONSTRUCTOR( TTrackerForAiming );
//    SO_NODE_ADD_FIELD( isAimingAbsolute, (TRUE) );
}

void TTrackerForAiming::setAimingAbsolute(bool enable)
{
    if ((isAimingAbsolute.getValue() == TRUE) == enable) return;
    isAimingAbsolute = enable ? TRUE : FALSE;
}
