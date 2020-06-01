#pragma once

#include "kernel/trackers/Tracker.h"

class TroughModel;
class SoNodeSensor;
class SoSensor;


class TrackerTrough: public Tracker
{
    SO_NODE_HEADER(TrackerTrough);

public:
    static void initClass();
    TrackerTrough();

    void update(TSeparatorKit* parent, const Transform& toGlobal, const Vector3D& vSun);

    // all vectors should be for zero rotation
    SoSFVec3f primaryShift;
    SoSFVec3f primaryAxis;
    SoSFVec2f primaryAngles;

    SoSFVec3f facetShift;
    SoSFVec3f facetNormal;

    enum AimingFrame {
        global = 0,
        primary = 1
    };
    SoSFEnum aimingFrame;
    SoSFVec3f aimingPoint;

    SoSFFloat angleDefault;

    NAME_ICON_FUNCTIONS("Trough", ":/TrackerTrough.png")

protected:
    ~TrackerTrough();

    TroughModel* m_trough;
    SoNodeSensor* m_sensor;
    static void onModified(void* data, SoSensor*);
};


class TrackerTroughFactory:
    public QObject, public TrackerFactoryT<TrackerTrough>
{
    Q_OBJECT
    Q_INTERFACES(TrackerFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.TrackerFactory")
};
