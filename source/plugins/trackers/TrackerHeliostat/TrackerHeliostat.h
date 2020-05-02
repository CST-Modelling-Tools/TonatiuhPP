#pragma once

#include "kernel/trackers/TTrackerForAiming.h"


class TrackerHeliostat: public TTrackerForAiming
{
    SO_NODEENGINE_HEADER(TrackerHeliostat);

public:
    static void initClass();
    TrackerHeliostat();

    void Evaluate(Vector3D sunVectorW, Transform parentWT0);

    enum Rotations {
		YX = 0,
        YZ = 1,
        XZ = 2,
        ZX = 3
	};

	SoSFEnum typeOfRotation;
	trt::TONATIUH_REALVECTOR3 aimingPoint;

    NAME_ICON_FUNCTIONS("TrackerHeliostat", ":/TrackerHeliostat.png")

protected:
    ~TrackerHeliostat() {}

private:
    void evaluate();

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
