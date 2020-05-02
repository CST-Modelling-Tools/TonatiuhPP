#pragma once

#include "TrackerAbstract.h"

class QString; 
class SoFieldSensor;
class SoSensor;


class TONATIUH_KERNEL TDefaultTracker: public TrackerAbstract
{
    SO_NODEENGINE_HEADER(TDefaultTracker);
    
public:
    static void initClass();
    QString getIcon()
    {
        return "";
    }
    
    TDefaultTracker();

protected:    
    ~TDefaultTracker();

private:
    void evaluate();
};
