#pragma once

#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/fields/SoSFVec3f.h>

#include "kernel/raytracing/TTracker.h"
#include "kernel/raytracing/trt.h"

class QString;

class GraphicRootTracker: public TTracker
{
    SO_NODEENGINE_HEADER(GraphicRootTracker);

public:
    static void initClass();
    QString getIcon();

    //Constructor
    GraphicRootTracker();

    void Disconnect();
    void SetAzimuthAngle(trt::TONATIUH_REAL* azimuthField);
    void SetZenithAngle(trt::TONATIUH_REAL* zenithField);

protected:
    virtual ~GraphicRootTracker();

private:
    virtual void evaluate();
};
