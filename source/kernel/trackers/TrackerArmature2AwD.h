#pragma once

#include "kernel/trackers/TrackerArmature2A.h"

// 2 axis with drives
class TONATIUH_KERNEL TrackerArmature2AwD: public TrackerArmature2A
{
    SO_NODE_HEADER(TrackerArmature2AwD);

public:
    static void initClass();
    TrackerArmature2AwD();

    void update(TSeparatorKit* parent, const Transform& toGlobal,
                const vec3d& vSun, TrackerTarget* target);

    void updateShape(TSeparatorKit* parent, SoShapeKit* shape, TrackerTarget* target);

    SoSFVec3f drivePrimaryR;
    SoSFVec3f drivePrimaryP;

    SoSFVec3f driveSecondaryP;
    SoSFVec3f driveSecondaryS;

    NAME_ICON_FUNCTIONS("two-axes with drives", ":/trackers/Tracker2A.png")
};

