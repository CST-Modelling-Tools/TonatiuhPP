#pragma once

#include "kernel/raytracing/TFactory.h"

class TTracker;



class TrackerFactory: public TFactory
{
public:
    virtual TTracker* create() const = 0;
};

Q_DECLARE_INTERFACE( TrackerFactory, "tonatiuh.TrackerFactory")
