#include "SunPosition.h"

#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoTransparencyType.h>

#include "libraries/math/gcf.h"

SO_NODE_SOURCE(SunPosition)


void SunPosition::initClass()
{
    SO_NODE_INIT_CLASS(SunPosition, SoNode, "Node");
}

SunPosition::SunPosition()
{
    SO_NODE_CONSTRUCTOR(SunPosition);
    isBuiltIn = TRUE;

    SO_NODE_ADD_FIELD( azimuth, (0.) );
    SO_NODE_ADD_FIELD( elevation, (90.) );
    SO_NODE_ADD_FIELD( irradiance, (1000.) );

    m_sensor = new SoNodeSensor(update, this);
    m_sensor->attach(this);
}

SunPosition::~SunPosition()
{
    delete m_sensor;
}

vec3d SunPosition::getSunVector()
{
    double gamma = azimuth.getValue()*gcf::degree;
    double alpha = elevation.getValue()*gcf::degree;

    return vec3d(
        sin(gamma)*cos(alpha),
        cos(gamma)*cos(alpha),
        sin(alpha)
    );
}

void SunPosition::update(void* /*data*/, SoSensor*)
{

}
