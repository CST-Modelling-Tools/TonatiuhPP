#pragma once


#include "kernel/trackers/TTrackerForAiming.h"


class TrackerTrough: public TTrackerForAiming
{
    SO_NODE_HEADER(TrackerTrough);

public:
    static void initClass();
    TrackerTrough();

    void Evaluate(SoNode* parent, const Transform& transform, const Vector3D& vSun);

    enum Axis {
		X = 0,
		Y = 1,
		Z = 2,
	};
    SoSFEnum activeAxis;

	trt::TONATIUH_REALVECTOR2 axisOrigin;

    NAME_ICON_FUNCTIONS("Trough", ":/TrackerTrough.png")

protected:
    ~TrackerTrough() {}
};



#include "kernel/trackers/TrackerFactory.h"

class TrackerTroughFactory:
    public QObject, public TrackerFactoryT<TrackerTrough>
{
    Q_OBJECT
    Q_INTERFACES(TrackerFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.TrackerFactory")
};
