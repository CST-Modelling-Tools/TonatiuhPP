#pragma once

#include "kernel/trackers/TrackerArmature.h"
#include "kernel/trackers/ArmatureJoint.h"

class TrackerSolver2A;
class SoNodeSensor;
class SoSensor;


class TONATIUH_KERNEL TrackerArmature2A: public TrackerArmature
{
    SO_NODE_HEADER(TrackerArmature2A);

public:
    static void initClass();
    TrackerArmature2A();

    void update(TSeparatorKit* parent, const Transform& toGlobal,
                const vec3d& vSun, TrackerTarget* target);

    // all vectors should be for zero rotation
    SoSFVec3f primaryShift;
    SoSFVec3f primaryAxis;
    SoSFVec2f primaryAngles;

    SoSFVec3f secondaryShift;
    SoSFVec3f secondaryAxis;
    SoSFVec2f secondaryAngles;

    SoSFVec3f facetShift;
    SoSFVec3f facetNormal;

    SoSFVec2f anglesDefault;

    ArmatureJoint primary;
    ArmatureJoint secondary;
    ArmatureVertex facet;
    vec2d angles0;

    NAME_ICON_FUNCTIONS("two-axes", ":/trackers/Tracker2A.png")

protected:
    ~TrackerArmature2A();

    TrackerSolver2A* m_solver;

    SoNodeSensor* m_sensor;
    static void onModified(void* data, SoSensor*);
    void onModified();
};
