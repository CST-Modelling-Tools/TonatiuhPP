#pragma once

#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodekits/SoBaseKit.h>
#include "kernel/scene/TTransform.h"


class TONATIUH_KERNEL TSeparatorKit: public SoBaseKit
{
    SO_KIT_HEADER(TSeparatorKit);

    SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
    SO_KIT_CATALOG_ENTRY_HEADER(transform);
    SO_KIT_CATALOG_ENTRY_HEADER(components);
    SO_KIT_CATALOG_ENTRY_HEADER(group);

public:
    static void initClass();
    TSeparatorKit();
//    TSeparatorKit* copy(SbBool copyConnections) const;

    void getBoundingBox(SoGetBoundingBoxAction* action);
//    void setCulling(bool on);

protected:
    ~TSeparatorKit() {}
};
