#pragma once

#include "kernel/TonatiuhKernel.h"
#include "TTracker.h"

class QString; 
class SoFieldSensor;
class SoSensor;

class TONATIUH_KERNEL TDefaultTracker : public TTracker
{
    SO_NODEENGINE_HEADER( TDefaultTracker );
    
public:
    static void initClass();
    QString getIcon();
    
    //Constructor
    TDefaultTracker();

protected:    
    virtual ~TDefaultTracker();
private:
  virtual void evaluate();

};
