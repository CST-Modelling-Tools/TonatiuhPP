#pragma once

#include "kernel/tracker/TTracker.h"

class QString;


class TONATIUH_KERNEL GraphicRootTracker: public TTracker
{
    SO_NODEENGINE_HEADER(GraphicRootTracker);

public:
    static void initClass();
    QString getIcon() {return "";}

    GraphicRootTracker();

    void SetAzimuthAngle(trt::TONATIUH_REAL* azimuthField);
    void SetZenithAngle(trt::TONATIUH_REAL* zenithField);
    void Disconnect();

protected:
    ~GraphicRootTracker();

private:
    void evaluate();
};
