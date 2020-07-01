#pragma once

#include "kernel/trackers/Tracker.h"

class TrackerDualSolver;
class SoNodeSensor;
class SoSensor;


class TrackerDual: public Tracker
{
    SO_NODE_HEADER(TrackerDual);

public:
    static void initClass();
    TrackerDual();

    void update(TSeparatorKit* parent, const Transform& toGlobal, const vec3d& vSun);

    // all vectors should be for zero rotation
    SoSFVec3f primaryShift;
    SoSFVec3f primaryAxis;
    SoSFVec2f primaryAngles;

    SoSFVec3f secondaryShift;
    SoSFVec3f secondaryAxis;
    SoSFVec2f secondaryAngles;

    SoSFVec3f facetShift;
    SoSFVec3f facetNormal;

    enum AimingFrame {
        global = 0,
        primary = 1,
        secondary = 2
    };
    SoSFEnum aimingFrame;
    SoSFVec3f aimingPoint;

    SoSFVec2f anglesDefault;

    NAME_ICON_FUNCTIONS("Dual", ":/TrackerDual.png")

protected:
    ~TrackerDual();

    TrackerDualSolver* m_heliostat;
    SoNodeSensor* m_sensor;
    static void onModified(void* data, SoSensor*);
};


class TrackerHeliostatFactory:
    public QObject, public TrackerFactoryT<TrackerDual>
{
    Q_OBJECT
    Q_INTERFACES(TrackerFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.TrackerFactory")
};
