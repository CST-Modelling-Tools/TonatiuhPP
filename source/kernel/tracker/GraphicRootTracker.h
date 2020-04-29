#pragma once

#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/fields/SoSFVec3f.h>

#include "kernel/tracker/TTracker.h"
#include "kernel/raytracing/trt.h"

class QString;

class TONATIUH_KERNEL GraphicRootTracker: public TTracker
{
    SO_NODEENGINE_HEADER(GraphicRootTracker);

public:
    static void initClass();
    QString getIcon();

    GraphicRootTracker();

    void Disconnect();
    void SetAzimuthAngle(trt::TONATIUH_REAL* azimuthField);
    void SetZenithAngle(trt::TONATIUH_REAL* zenithField);

protected:
    virtual ~GraphicRootTracker();

private:
    virtual void evaluate();
};
