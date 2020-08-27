#pragma once

#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodekits/SoBaseKit.h>

class TSeparatorKit;
class Transform;
struct vec3d;


class TONATIUH_KERNEL TrackerKit: public SoBaseKit
{
    SO_KIT_HEADER(TrackerKit);

public:
    static void initClass();
    TrackerKit();

    SoSFNode armature;
    SoSFNode target;

    void update(TSeparatorKit* parent, const Transform& toGlobal, const vec3d& vSun);
};

