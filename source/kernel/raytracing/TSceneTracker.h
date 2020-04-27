#pragma once

#include "kernel/TonatiuhKernel.h"
#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/fields/SoSFVec3f.h>

#include "kernel/tracker/TTracker.h"
#include "trt.h"

class QString;

class TONATIUH_KERNEL TSceneTracker: public TTracker
{
    SO_NODEENGINE_HEADER(TSceneTracker);

public:
    static void initClass();
    QString getIcon();

    //Constructor
    TSceneTracker();


protected:
    virtual ~TSceneTracker();

private:
    virtual void evaluate();

    //trt::TONATIUH_REAL m_azimuth;
    //trt::TONATIUH_REAL m_zenith;

};
