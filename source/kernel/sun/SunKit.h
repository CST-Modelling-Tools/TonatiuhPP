#pragma once

#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodekits/SoBaseKit.h>

#include "kernel/TonatiuhTypes.h"
#include "libraries/math/3D/Box3D.h"

class InstanceNode;
class TSceneKit;
class SoTransform;
class SoMaterial;
class SoTexture2;


class TONATIUH_KERNEL SunKit: public SoBaseKit
{
    SO_KIT_HEADER(SunKit);

    SO_KIT_CATALOG_ENTRY_HEADER(position);
    SO_KIT_CATALOG_ENTRY_HEADER(shape);
    SO_KIT_CATALOG_ENTRY_HEADER(aperture);

public:
    static void initClass(void);
    SunKit();

    void updateTransform();
    void setBox(Box3D box);
    void setBox(TSceneKit* scene);
    bool findTexture(int sizeX, int sizeY, InstanceNode* instanceRoot);

public:
    SoMaterial* m_imageMaterial;
    SoTexture2* m_imageTexture;
    SoTransform* m_transform;
};
