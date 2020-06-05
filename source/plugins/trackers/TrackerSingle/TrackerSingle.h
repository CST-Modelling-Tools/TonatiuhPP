#pragma once

#include "kernel/trackers/Tracker.h"

class TrackerSingleSolver;
class SoNodeSensor;
class SoSensor;


class TrackerSingle: public Tracker
{
    SO_NODE_HEADER(TrackerSingle);

public:
    static void initClass();
    TrackerSingle();

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

    NAME_ICON_FUNCTIONS("Single", ":/TrackerSingle.png")

protected:
    ~TrackerSingle();

    TrackerSingleSolver* m_trough;
    SoNodeSensor* m_sensor;
    static void onModified(void* data, SoSensor*);
};


class TrackerTroughFactory:
    public QObject, public TrackerFactoryT<TrackerSingle>
{
    Q_OBJECT
    Q_INTERFACES(TrackerFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.TrackerFactory")
};
