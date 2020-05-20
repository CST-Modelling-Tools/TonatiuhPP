#pragma once

#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/actions/SoSearchAction.h>

#include "TSeparatorKit.h"
#include "kernel/TonatiuhKernel.h"
#include "kernel/TonatiuhTypes.h"
#include "kernel/TonatiuhFunctions.h"

struct Vector3D;
class Transform;


class TONATIUH_KERNEL TSceneKit: public SoSceneKit
{
    typedef SoSceneKit inherited;

    SO_KIT_HEADER(TSceneKit);
    SO_KIT_CATALOG_ENTRY_HEADER(transmissivity);

public:
    TSceneKit();
    static void initClass();

    SoPath* GetSoPath(SoSearchAction* action);

    void updateTrackers();

    SoSeparator* m_root; // graphicRoot

protected:
    ~TSceneKit() {}
    void updateTrackers(SoBaseKit* branch, Transform toGlobal, const Vector3D& vSun);
};
