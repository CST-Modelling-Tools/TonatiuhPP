#pragma once
#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodekits/SoBaseKit.h>


class TONATIUH_KERNEL WorldKit: public SoBaseKit
{
    SO_KIT_HEADER(WorldKit);

    SO_KIT_CATALOG_ENTRY_HEADER(location);

public:
    static void initClass(void);
    WorldKit();
};

