#pragma once
#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodes/SoSubNode.h>

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFVec3d.h>
#include <Inventor/fields/SoSFVec3i32.h>
#include <Inventor/fields/SoSFString.h>


class TONATIUH_KERNEL GridNode: public SoNode
{
    SO_NODE_HEADER(GridNode);

public:
    static void initClass();
    GridNode();

    SoSFBool grid;
    SoSFBool fill;

    SoSFVec3d steps;
    SoSFVec3i32 divisions;
    SoSFVec3d min;
    SoSFVec3d max;

    SoSFString file;
};

