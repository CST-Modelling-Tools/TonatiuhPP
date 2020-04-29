#pragma once

#include "TTracker.h"

class QString; 
class SoFieldSensor;
class SoSensor;


class TONATIUH_KERNEL TDefaultTracker: public TTracker
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
