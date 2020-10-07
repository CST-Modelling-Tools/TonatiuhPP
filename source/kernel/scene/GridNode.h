#pragma once
#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFVec2d.h>


class TONATIUH_KERNEL GridNode: public SoNode
{
    SO_NODE_HEADER(GridNode);

public:
    static void initClass();
    GridNode();

    SoSFBool show;
    SoSFBool fill;
    SoSFDouble step;
    SoSFInt32 divisions;
    SoSFVec2d xRange;
    SoSFVec2d yRange;
};

