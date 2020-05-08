#pragma once

#include "kernel/scene/TAbstract.h"

#include <Inventor/engines/SoNodeEngine.h>
#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/nodes/SoTransform.h>

#include "kernel/TonatiuhTypes.h"
#include "libraries/geometry/Vector3D.h"

class QString;
class TSceneKit;
class TLightKit;
class Transform;
struct Vector3D;


class TONATIUH_KERNEL TrackerAbstract: public SoNodeEngine
{
    SO_NODEENGINE_ABSTRACT_HEADER(TrackerAbstract);

public:
    static void initClass();
    virtual QString getIcon();

    void SetSceneKit(TSceneKit* scene);
    void SetAzimuthAngle(trt::TONATIUH_REAL* azimuthField);
    void SetZenithAngle(trt::TONATIUH_REAL* zenithField);
    void Disconnect();
    void ConnectParentTranform(SoTransform* parentTransform);
    virtual void Evaluate(const Vector3D& /*vSunW*/, const Transform& /*transformWtO*/) {}

    //void SetLightAngles(TLightKit * coinLight );
    //void SetAnglesToScene();
    //double GetAzimuth() { return m_azimuth.getValue();};
    //double GetZenith() { return m_zenith.getValue();};

    NAME_ICON_FUNCTIONS("X", ":/TrackerX.png")

protected:
    TrackerAbstract();
    ~TrackerAbstract();

    void SetEngineOutputIdentity();
    void SetEngineOutput(SoTransform* newTransform);
    void SetEngineOutputRotation(SbRotation rotation);
//    Vector3D GetGobalSunVector();
    //bool IsConnected();

    TSceneKit* m_scene;

    trt::TONATIUH_REAL m_azimuth;
    trt::TONATIUH_REAL m_zenith;

    SoEngineOutput outputTranslation;
    SoEngineOutput outputRotation;
    SoEngineOutput outputScaleFactor;
    SoEngineOutput outputScaleOrientation;
    SoEngineOutput outputCenter;

private:
    void evaluate() {}
};