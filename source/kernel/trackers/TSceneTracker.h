#pragma once

#include "kernel/trackers/TrackerAbstract.h"

class QString;


class TONATIUH_KERNEL TSceneTracker: public TrackerAbstract
{
    SO_NODEENGINE_HEADER(TSceneTracker);

public:
    static void initClass();

    TSceneTracker();

protected:
    ~TSceneTracker();

private:
    void evaluate();
};
