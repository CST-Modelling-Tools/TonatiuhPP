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
#include "kernel/scene/TSceneKit.h"
#include "libraries/geometry/Vector3D.h"

SO_NODEENGINE_SOURCE(GraphicRootTracker)


void GraphicRootTracker::initClass()
{
    SO_NODEENGINE_INIT_CLASS(GraphicRootTracker, TrackerAbstract, "TrackerAbstract");
}

GraphicRootTracker::GraphicRootTracker()
{
    SO_NODEENGINE_CONSTRUCTOR(GraphicRootTracker);

    SO_NODE_ADD_FIELD( m_azimuth, (0.) );
    SO_NODE_ADD_FIELD( m_zenith, (90.) );

    SO_NODEENGINE_ADD_OUTPUT(outputTranslation, SoSFVec3f);
    SO_NODEENGINE_ADD_OUTPUT(outputRotation, SoSFRotation);
    SO_NODEENGINE_ADD_OUTPUT(outputScaleFactor, SoSFVec3f);
    SO_NODEENGINE_ADD_OUTPUT(outputScaleOrientation, SoSFRotation);
    SO_NODEENGINE_ADD_OUTPUT(outputCenter, SoSFVec3f);
}

GraphicRootTracker::~GraphicRootTracker()
{
    Disconnect();
}

void GraphicRootTracker::Disconnect()
{
    m_azimuth.disconnect();
    m_zenith.disconnect();
}

void GraphicRootTracker::evaluate()
{
    if (!m_azimuth.isConnected() || !m_zenith.isConnected()) return;

    double alpha = gc::Pi - m_azimuth.getValue();
    SbRotation yRotation(SbVec3f(0., 0., -1.), alpha);

    double zenith = m_zenith.getValue();
    SbRotation xRotation(SbVec3f(1., 0., 0.), zenith);

    SbRotation rotation = xRotation*yRotation;
    SetEngineOutputRotation(rotation);
}
