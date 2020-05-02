#pragma once

#include "kernel/TonatiuhKernel.h"
#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/actions/SoSearchAction.h>
#include "TSeparatorKit.h"
#include "kernel/TonatiuhTypes.h"
#include "kernel/TonatiuhFunctions.h"

class QString;
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

    trt::TONATIUH_REAL* GetAzimuthAngle();
    trt::TONATIUH_REAL* GetZenithAngle();

    SoPath* GetSoPath(SoSearchAction* action);

    void UpdateSunPosition(double azimuthValue, double zenithValue);

    trt::TONATIUH_REAL azimuth;
    trt::TONATIUH_REAL zenith;

protected:
    ~TSceneKit() {}
    void UpdateTrackersTransform(SoBaseKit* branch, Vector3D sunVector, Transform parentOTW);
};
