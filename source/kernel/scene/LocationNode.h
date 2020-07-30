#pragma once
#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFDouble.h>


class TONATIUH_KERNEL LocationNode: public SoNode
{
    SO_NODE_HEADER(LocationNode);

public:
    static void initClass();
    LocationNode();

    SoSFString name;
    SoSFDouble latitude;
    SoSFDouble longitude;
};

