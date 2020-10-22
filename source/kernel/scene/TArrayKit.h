#pragma once

#include "kernel/TonatiuhKernel.h"
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoSeparator.h>

#include "kernel/scene/TSeparatorKit.h"

class SoFieldSensor;
class SoSensor;


class TONATIUH_KERNEL TArrayKit: public SoNode
{
//    SO_KIT_HEADER(TArrayKit);
    SO_NODE_HEADER(TArrayKit);

public:
    static void initClass();
    TArrayKit();

    SoMFVec3f positions;
    SoSFInt32 nMax;

    TSeparatorKit* m_parent;

    void GLRender(SoGLRenderAction* action);

protected:
     ~TArrayKit();

    SoFieldSensor* m_sensor;
    SoSeparator* m_root;
    static void onSensor(void* data, SoSensor*);
    void onSensor();

    // SoNode interface
public:
    void getBoundingBox(SoGetBoundingBoxAction* action);
};

