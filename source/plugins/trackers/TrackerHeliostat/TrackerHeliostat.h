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

    SoSFVec3f primaryShift;
    SoSFVec3f primaryAxis;

    SoSFVec3f secondaryShift;
    SoSFVec3f secondaryAxis;

    SoSFVec3f mirrorPoint;
    SoSFVec3f mirrorNormal;

    SoSFEnum aimingType;
    SoSFVec3f aimingPoint;

    NAME_ICON_FUNCTIONS("Heliostat", ":/TrackerHeliostat.png")

protected:
    ~TrackerHeliostat();

    HeliostatModel* m_hm;
    SoNodeSensor* m_sensor;
    static void update(void* data, SoSensor*);
};



#include "kernel/trackers/TrackerFactory.h"

class TrackerHeliostatFactory:
    public QObject, public TrackerFactoryT<TrackerHeliostat>
{
    Q_OBJECT
    Q_INTERFACES(TrackerFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.TrackerFactory")
};
