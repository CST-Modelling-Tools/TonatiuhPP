#pragma once

#include "TrackerAbstract.h"


class TONATIUH_KERNEL TDefaultTracker: public TrackerAbstract
{
    SO_NODE_HEADER(TDefaultTracker);
    
public:
    static void initClass();  
    TDefaultTracker();

protected:    
    ~TDefaultTracker() {}
};
