#pragma once

#include "kernel/tracker/TTracker.h"
//#include <Inventor/fields/SoSFVec3f.h>


class QString;

class TONATIUH_KERNEL TSceneTracker: public TTracker
{
    SO_NODEENGINE_HEADER(TSceneTracker);

public:
    static void initClass();
    QString getIcon();

    TSceneTracker();

protected:
    virtual ~TSceneTracker();

private:
    virtual void evaluate();

    //trt::TONATIUH_REAL m_azimuth;
    //trt::TONATIUH_REAL m_zenith;
};
