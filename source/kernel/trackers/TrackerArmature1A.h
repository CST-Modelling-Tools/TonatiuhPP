#pragma once

#include "kernel/trackers/TrackerArmature.h"
#include "kernel/trackers/ArmatureJoint.h"

class TrackerSolver1A;
class SoNodeSensor;
class SoSensor;


class TONATIUH_KERNEL TrackerArmature1A: public TrackerArmature
{
    SO_NODE_HEADER(TrackerArmature1A);

public:
    static void initClass();
    TrackerArmature1A();

    void update(TSeparatorKit* parent, const Transform& toGlobal,
                const vec3d& vSun, TrackerTarget* target);

    // all vectors should be for zero rotation
    SoSFVec3f primaryShift;
    SoSFVec3f primaryAxis;
    SoSFVec2f primaryAngles;

    SoSFVec3f facetShift;
    SoSFVec3f facetNormal;

    SoSFFloat angleDefault;

    ArmatureJoint primary;
    ArmatureVertex facet;
    double angle0;

    NAME_ICON_FUNCTIONS("one-axis", ":/images/Tracker1A.png")

protected:
    ~TrackerArmature1A();

    TrackerSolver1A* m_solver;

    SoNodeSensor* m_sensor;
    static void onModified(void* data, SoSensor*);
    void onModified();
};
