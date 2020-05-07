#pragma once

#include "kernel/trackers/TTrackerForAiming.h"


class TrackerHeliostat: public TTrackerForAiming
{
    SO_NODEENGINE_HEADER(TrackerHeliostat);

public:
    static void initClass();
    TrackerHeliostat();

    void Evaluate(const Vector3D& vSunW, const Transform& transformWtO);

    SoSFVec3f primaryAxis;
    SoSFVec3f secondaryAxis;
    SoSFVec3f mirrorNormal;

    SoSFVec3f aimingPoint;

    NAME_ICON_FUNCTIONS("Heliostat", ":/TrackerHeliostat.png")

protected:
    ~TrackerHeliostat() {}

private:
	int m_previousAimingPointType;
    SoFieldSensor* m_sensorAiming;
};


#include "kernel/trackers/TrackerFactory.h"

class TrackerHeliostatFactory:
    public QObject, public TrackerFactoryT<TrackerHeliostat>
{
    Q_OBJECT
    Q_INTERFACES(TrackerFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.TrackerFactory")
};
