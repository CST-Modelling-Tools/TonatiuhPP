#pragma once

#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodekits/SoSeparatorKit.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFVec4f.h>

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

//    SoSFVec3f translation;
//    SoSFVec4f rotation;
//    SoSFVec3f scale;

    void getBoundingBox(SoGetBoundingBoxAction* action);

protected:
    ~TSeparatorKit() {}
};
