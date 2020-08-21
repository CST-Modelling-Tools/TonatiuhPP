#pragma once

#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodekits/SoSeparatorKit.h>


class TONATIUH_KERNEL TSeparatorKit: public SoBaseKit
{
    SO_KIT_HEADER(TSeparatorKit);

    SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
    SO_KIT_CATALOG_ENTRY_HEADER(transform);
    SO_KIT_CATALOG_ENTRY_HEADER(tracker);
    SO_KIT_CATALOG_ENTRY_HEADER(group);

public:
    static void initClass();
    TSeparatorKit();

    void getBoundingBox(SoGetBoundingBoxAction* action);

protected:
    ~TSeparatorKit() {}
};
