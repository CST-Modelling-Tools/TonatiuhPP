#pragma once

#include "kernel/tracker/TTracker.h"

class QString;


class TONATIUH_KERNEL TSceneTracker: public TTracker
{
    SO_NODEENGINE_HEADER(TSceneTracker);

public:
    static void initClass();

    TSceneTracker();

protected:
    ~TSceneTracker();

private:
    virtual void evaluate();
};
