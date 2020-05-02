#pragma once

#include "kernel/trackers/TrackerAbstract.h"

class QString;


class TONATIUH_KERNEL GraphicRootTracker: public TrackerAbstract
{
    SO_NODEENGINE_HEADER(GraphicRootTracker);

public:
    static void initClass();

    GraphicRootTracker();

    void Disconnect();

protected:
    ~GraphicRootTracker();

private:
    void evaluate();
};
