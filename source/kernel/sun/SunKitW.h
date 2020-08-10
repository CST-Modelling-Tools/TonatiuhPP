#pragma once
#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodekits/SoBaseKit.h>

#include "kernel/TonatiuhTypes.h"
#include "libraries/math/3D/Box3D.h"
class InstanceNode;
class TSceneKit;

class TONATIUH_KERNEL SunKitW: public SoBaseKit
{
    SO_KIT_HEADER(SunKitW);

    SO_KIT_CATALOG_ENTRY_HEADER(iconMaterial);
    SO_KIT_CATALOG_ENTRY_HEADER(iconTexture);
    SO_KIT_CATALOG_ENTRY_HEADER(transform);

    SO_KIT_CATALOG_ENTRY_HEADER(position);
    SO_KIT_CATALOG_ENTRY_HEADER(shape);
    SO_KIT_CATALOG_ENTRY_HEADER(aperture);

public:
    static void initClass(void);
    SunKitW();

    void updateTransform();
    void setBox(Box3D box);
    void setBox(TSceneKit* scene);
    bool findTexture(int sizeX, int sizeY, InstanceNode* instanceRoot);
};
