#pragma once

#include <Inventor/nodekits/SoSceneKit.h>
//#include <Inventor/actions/SoSearchAction.h>

#include "kernel/TonatiuhKernel.h"
#include "kernel/TonatiuhTypes.h"
#include "kernel/TonatiuhFunctions.h"

class Transform;
struct Vector3D;
class TSeparatorKit;


class TONATIUH_KERNEL TSceneKit: public SoSceneKit
{
    SO_KIT_HEADER(TSceneKit);
    SO_KIT_CATALOG_ENTRY_HEADER(group);
    SO_KIT_CATALOG_ENTRY_HEADER(air);

public:
    static void initClass();
    TSceneKit();

    void updateTrackers();

    SoSeparator* m_root; // graphicRoot

protected:
    ~TSceneKit() {}
    void updateTrackers(TSeparatorKit* parent, Transform t, const Vector3D& vSun);
};
