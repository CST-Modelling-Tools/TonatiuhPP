#include <cmath>
#include <iostream>

#include <QString>

#include <Inventor/SbLinear.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoSceneKit.h>

#include "libraries/geometry/gc.h"

#include "libraries/geometry/NormalVector.h"
#include "libraries/geometry/Point3D.h"
#include "GraphicRootTracker.h"
#include "libraries/geometry/Transform.h"
#include "kernel/raytracing/TSceneKit.h"
#include "libraries/geometry/Vector3D.h"

SO_NODEENGINE_SOURCE( GraphicRootTracker );

void GraphicRootTracker::initClass()
{
    SO_NODEENGINE_INIT_CLASS( GraphicRootTracker, TTracker, "TTracker" );

}

GraphicRootTracker::GraphicRootTracker()
{
    SO_NODEENGINE_CONSTRUCTOR( GraphicRootTracker );

    // Define input fields and their default values
    SO_NODE_ADD_FIELD( m_azimuth, ( 0.0 ) );
    SO_NODE_ADD_FIELD( m_zenith, ( 90.0 ) );

    //ConstructEngineOutput();
    SO_NODEENGINE_ADD_OUTPUT( outputTranslation, SoSFVec3f);
    SO_NODEENGINE_ADD_OUTPUT( outputRotation, SoSFRotation);
    SO_NODEENGINE_ADD_OUTPUT( outputScaleFactor, SoSFVec3f);
    SO_NODEENGINE_ADD_OUTPUT( outputScaleOrientation, SoSFRotation);
    SO_NODEENGINE_ADD_OUTPUT( outputCenter, SoSFVec3f);


}

GraphicRootTracker::~GraphicRootTracker()
{
    m_azimuth.disconnect();
    m_zenith.disconnect();
}

void GraphicRootTracker::Disconnect()
{
    m_azimuth.disconnect();
    m_zenith.disconnect();
}

void GraphicRootTracker::SetAzimuthAngle( trt::TONATIUH_REAL* azimuthField )
{
    m_azimuth.connectFrom(azimuthField);
}

void GraphicRootTracker::SetZenithAngle( trt::TONATIUH_REAL* zenithField )
{
    m_zenith.connectFrom(zenithField);
}


QString GraphicRootTracker::getIcon()
{

    return QString( QLatin1String( ":/icons/GraphicRootTracker.png" ) );
}

void GraphicRootTracker::evaluate()
{

    if (!m_azimuth.isConnected() || !m_zenith.isConnected() ) return;

    //double alpha = gc::Pi - GetAzimuth();
    double azimuth = m_azimuth.getValue();
    double alpha = gc::Pi - azimuth;

    SbVec3f yAxis( 0.0, 1.0, 0.0 );
    SbRotation yRotation( yAxis, alpha );
    SbVec3f xAxis( 1.0, 0.0, 0.0 );
    //SbRotation xRotation( xAxis, GetZenith() );
    double zenith = m_zenith.getValue();
    SbRotation xRotation( xAxis, zenith );

    SbRotation rotation = xRotation * yRotation;

    SetEngineOutputRotation(rotation);
}
