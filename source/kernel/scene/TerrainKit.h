#pragma once
#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodekits/SoBaseKit.h>


class TONATIUH_KERNEL TerrainKit: public SoBaseKit
{
    SO_KIT_HEADER(TerrainKit);

    SO_KIT_CATALOG_ENTRY_HEADER(grid);

public:
    static void initClass(void);
    TerrainKit();
};

