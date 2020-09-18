#pragma once
#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/fields/SoSFBool.h>


class TONATIUH_KERNEL MaterialGL: public SoMaterial
{
    SO_NODE_HEADER(MaterialGL);

public:
    static void initClass();
    MaterialGL();

    SoSFBool reverseNormals;
};

