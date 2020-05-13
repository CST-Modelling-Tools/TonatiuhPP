#pragma once

#include "kernel/trackers/TrackerAbstract.h"


class TrackerTrough: public TrackerAbstract
{
    SO_NODE_HEADER(TrackerTrough);

public:
    static void initClass();
    TrackerTrough();

    void Evaluate(SoBaseKit* parent, const Transform& toGlobal, const Vector3D& vSun);

    SoSFVec3f trackingAxis;
    SoSFVec3f mirrorNormal;

    SoSFBool isAimingAbsolute;
    SoSFVec3f aimingPoint;

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
