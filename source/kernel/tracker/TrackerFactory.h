#pragma once

#include "kernel/scene/TFactory.h"

class TTracker;



class TrackerFactory: public TFactory
{
public:
    virtual TTracker* create() const = 0;
};

Q_DECLARE_INTERFACE( TrackerFactory, "tonatiuh.TrackerFactory")
