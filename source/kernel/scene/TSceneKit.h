#pragma once

#include <Inventor/nodekits/SoSceneKit.h>

#include "kernel/TonatiuhKernel.h"
#include "kernel/TonatiuhTypes.h"
#include "kernel/TonatiuhFunctions.h"

class TSeparatorKit;
class Transform;
struct vec3d;
class GraphicRoot;

class TONATIUH_KERNEL TSceneKit: public SoSceneKit
{
    SO_KIT_HEADER(TSceneKit);

    SoSFString version;
    SO_KIT_CATALOG_ENTRY_HEADER(world);
    SO_KIT_CATALOG_ENTRY_HEADER(group); // scene

public:
    static void initClass();
    TSceneKit();

    TSeparatorKit* getLayout();

    void updateTrackers();

    GraphicRoot* m_graphicRoot;

protected:
    ~TSceneKit() {}
    void updateTrackers(TSeparatorKit* parent, Transform t, const vec3d& vSun);
};
