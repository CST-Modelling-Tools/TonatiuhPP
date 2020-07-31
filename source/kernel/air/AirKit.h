#pragma once
#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodekits/SoBaseKit.h>


class TONATIUH_KERNEL AirKit: public SoBaseKit
{
    SO_KIT_HEADER(AirKit);

    SO_KIT_CATALOG_ENTRY_HEADER(transmission);

public:
    static void initClass(void);
    AirKit();
};
