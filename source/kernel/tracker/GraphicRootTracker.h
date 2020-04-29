#pragma once

#include "kernel/tracker/TTracker.h"

class QString;


class TONATIUH_KERNEL GraphicRootTracker: public TTracker
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
