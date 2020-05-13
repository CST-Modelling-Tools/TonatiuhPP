#pragma once

#include "kernel/trackers/TrackerAbstract.h"


class TrackerHeliostat: public TrackerAbstract
{
    SO_NODE_HEADER(TrackerHeliostat);

public:
    static void initClass();
    TrackerHeliostat();

    void Evaluate(SoNode* parent, const Transform& toGlobal, const Vector3D& vSun);

    SoSFVec3f primaryAxis;
    SoSFVec3f secondaryAxis;
    SoSFVec3f mirrorNormal;

    SoSFBool isAimingAbsolute;
    SoSFVec3f aimingPoint;

    NAME_ICON_FUNCTIONS("Heliostat", ":/TrackerHeliostat.png")

protected:
    ~TrackerHeliostat() {}
};



#include "kernel/trackers/TrackerFactory.h"

class TrackerHeliostatFactory:
    public QObject, public TrackerFactoryT<TrackerHeliostat>
{
    Q_OBJECT
    Q_INTERFACES(TrackerFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.TrackerFactory")
};
