#pragma once

#include <QtPlugin>

class QString;
class QIcon;
class TTracker;

class TTrackerFactory
{
public:
    virtual ~TTrackerFactory() {}
    virtual QString TTrackerName() const  = 0;
    virtual QIcon TTrackerIcon() const = 0;
    virtual TTracker* create( ) const = 0;
};

Q_DECLARE_INTERFACE( TTrackerFactory, "tonatiuh.TTrackerFactory")
