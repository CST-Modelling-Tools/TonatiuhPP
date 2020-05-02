#include "TrackerAbstract.h"

#include <QString>
#include "libraries/geometry/Transform.h"
#include "scene/TSceneKit.h"
#include "sun/TLightKit.h"
//#include "geometry/tgc.h"


SO_NODEENGINE_ABSTRACT_SOURCE(TrackerAbstract)


void TrackerAbstract::initClass()
{
    SO_NODEENGINE_INIT_ABSTRACT_CLASS(TrackerAbstract, SoNodeEngine, "NodeEngine");
}

QString TrackerAbstract::getIcon()
{
    return "";
}

TrackerAbstract::TrackerAbstract():
    m_scene(0)
{

}

TrackerAbstract::~TrackerAbstract()
{
    m_scene = 0;
    //m_azimuth.disconnect();
    //m_zenith.disconnect();
}

void TrackerAbstract::SetSceneKit(TSceneKit* scene)
{
    m_scene = scene;
}

void TrackerAbstract::SetAzimuthAngle(trt::TONATIUH_REAL* azimuthField)
{
    m_azimuth.connectFrom(azimuthField);
}

void TrackerAbstract::SetZenithAngle(trt::TONATIUH_REAL* zenithField)
{
    m_zenith.connectFrom(zenithField);
}

void TrackerAbstract::Disconnect()
{
    m_azimuth.disconnect();
    m_zenith.disconnect();
}

void TrackerAbstract::ConnectParentTranform(SoTransform* parentTransform)
{
    parentTransform->translation.connectFrom(&outputTranslation);
    parentTransform->rotation.connectFrom(&outputRotation);
    parentTransform->scaleFactor.connectFrom(&outputScaleFactor);
    parentTransform->scaleOrientation.connectFrom(&outputScaleOrientation);
    parentTransform->center.connectFrom(&outputCenter);
}

void TrackerAbstract::Evaluate(Vector3D, Transform) {}

void TrackerAbstract::SetEngineOutputIdentity()
{
    SO_ENGINE_OUTPUT(outputTranslation, SoSFVec3f, setValue(0., 0., 0.) );
    SO_ENGINE_OUTPUT(outputRotation, SoSFRotation, setValue(0., 0., 1.0, 0.) );
    SO_ENGINE_OUTPUT(outputScaleFactor, SoSFVec3f, setValue(1., 1., 1.) );
    SO_ENGINE_OUTPUT(outputScaleOrientation, SoSFRotation, setValue(0., 0., 1., 0.) );
    SO_ENGINE_OUTPUT(outputCenter, SoSFVec3f, setValue(0., 0., 0.) );
}

void TrackerAbstract::SetEngineOutput(SoTransform* newTransform)
{
    SO_ENGINE_OUTPUT(outputTranslation, SoSFVec3f, setValue(newTransform->translation.getValue() ) );
    SO_ENGINE_OUTPUT(outputRotation, SoSFRotation, setValue(newTransform->rotation.getValue() ) );
    SO_ENGINE_OUTPUT(outputScaleFactor, SoSFVec3f, setValue(newTransform->scaleFactor.getValue() ) );
    SO_ENGINE_OUTPUT(outputScaleOrientation, SoSFRotation, setValue(newTransform->scaleOrientation.getValue() ) );
    SO_ENGINE_OUTPUT(outputCenter, SoSFVec3f, setValue(newTransform->center.getValue() ) );
}

void TrackerAbstract::SetEngineOutputRotation(SbRotation rotation)
{
    SO_ENGINE_OUTPUT(outputTranslation, SoSFVec3f, setValue(SbVec3f(0., 0., 0.) ) );
    SO_ENGINE_OUTPUT(outputRotation, SoSFRotation, setValue(rotation) );
    SO_ENGINE_OUTPUT(outputScaleFactor, SoSFVec3f, setValue(SbVec3f(1., 1., 1.) ) );
    SO_ENGINE_OUTPUT(outputScaleOrientation, SoSFRotation, setValue(SbRotation() ) );
    SO_ENGINE_OUTPUT(outputCenter, SoSFVec3f, setValue(SbVec3f(0., 0., 0.) ) );
}



/*
   bool TTracker::IsConnected()
   {
    if( !m_azimuth.isConnected() ) return false;
    if( !m_zenith.isConnected() ) return false;
    return true;
   }

   Vector3D TTracker::GetGobalSunVector()
   {
    Vector3D globalSunVector( sin( m_azimuth.getValue() ) * sin( m_zenith.getValue() ),
                 cos( m_zenith.getValue() ),
                -cos( m_azimuth.getValue() ) * sin( m_zenith.getValue() ) );
    return globalSunVector;
   }

   void TTracker::SetAnglesToScene()
   {
    if( m_scene )
    {
        m_scene->azimuth.setValue( m_azimuth.getValue() );
        m_scene->zenith.setValue( m_zenith.getValue() );
    }
   }
 */

/*
   void TTracker::SetLightAngles(TLightKit * coinLight )
   {
    m_azimuth.connectFrom( &(coinLight->azimuth) );
    m_zenith.connectFrom( &(coinLight->zenith) );
   }
 */
