#pragma once

#include "kernel/trackers/TrackerAbstract.h"

class HeliostatModel;
class SoNodeSensor;
class SoSensor;


class TrackerHeliostat: public TrackerAbstract
{
    SO_NODE_HEADER(TrackerHeliostat);

public:
    static void initClass();
    TrackerHeliostat();

    void update(SoBaseKit* parent, const Transform& toGlobal, const Vector3D& vSun);

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

    NAME_ICON_FUNCTIONS("Heliostat", ":/TrackerHeliostat.png")

protected:
    ~TrackerHeliostat();

    HeliostatModel* m_heliostat;
    SoNodeSensor* m_sensor;
    static void onModified(void* data, SoSensor*);
};



#include "kernel/trackers/TrackerFactory.h"

class TrackerHeliostatFactory:
    public QObject, public TrackerFactoryT<TrackerHeliostat>
{
    Q_OBJECT
    Q_INTERFACES(TrackerFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.TrackerFactory")
};
