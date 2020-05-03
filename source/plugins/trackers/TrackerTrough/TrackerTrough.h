#pragma once

#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/fields/SoSFEnum.h>

#include "kernel/trackers/TTrackerForAiming.h"


class TrackerTrough: public TTrackerForAiming
{
    SO_NODEENGINE_HEADER(TrackerTrough);

public:
    static void initClass();

    TrackerTrough();
    void Evaluate(const Vector3D& vSunW, const Transform& transformWtO);

    enum Axis {
		X = 0,
		Y = 1,
		Z = 2,
	};
    SoSFEnum activeAxis;

	trt::TONATIUH_REALVECTOR2 axisOrigin;

    NAME_ICON_FUNCTIONS("TrackerTrough", ":/TrackerTrough.png")

protected:
    ~TrackerTrough() {}

private:
	int m_previousAimingPointType;
};



#include "kernel/trackers/TrackerFactory.h"

class TrackerTroughFactory:
    public QObject, public TrackerFactoryT<TrackerTrough>
{
    Q_OBJECT
    Q_INTERFACES(TrackerFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.TrackerFactory")
};
