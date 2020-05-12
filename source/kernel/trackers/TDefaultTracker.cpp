#include "TDefaultTracker.h"

SO_NODE_SOURCE(TDefaultTracker)


void TDefaultTracker::initClass()
{
    SO_NODE_INIT_CLASS(TDefaultTracker, TrackerAbstract, "TrackerAbstract");
}

TDefaultTracker::TDefaultTracker()
{
    SO_NODE_CONSTRUCTOR(TDefaultTracker);
}
