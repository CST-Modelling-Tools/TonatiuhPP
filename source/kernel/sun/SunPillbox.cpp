#include "SunPillbox.h"

#include "libraries/geometry/gc.h"
#include <Inventor/sensors/SoFieldSensor.h>

SO_NODE_SOURCE(SunPillbox)


void SunPillbox::initClass()
{
    SO_NODE_INIT_CLASS(SunPillbox, SunAbstract, "SunAbstract");
}

SunPillbox::SunPillbox()
{
    SO_NODE_CONSTRUCTOR(SunPillbox);
    SO_NODE_ADD_FIELD( irradiance, (1000.) );
    SO_NODE_ADD_FIELD( thetaMax, (0.00465) );
    updateTheta(this, 0);

    m_sensorTheta = new SoFieldSensor(updateTheta, this);
    m_sensorTheta->attach(&thetaMax);
}

SunPillbox::~SunPillbox()
{
    delete m_sensorTheta;
}

void SunPillbox::updateTheta(void* data, SoSensor*)
{
    SunPillbox* sun = (SunPillbox*) data;
    sun->m_sinThetaMax = sin(sun->thetaMax.getValue());
}

void SunPillbox::generateRay(Vector3D& direction, RandomAbstract& rand) const
{
    double phi = gc::TwoPi*rand.RandomDouble();
    double sinTheta = m_sinThetaMax*sqrt(rand.RandomDouble());
    double cosTheta = sqrt(1. - sinTheta*sinTheta);
    double cosPhi = cos(phi);
    double sinPhi = sin(phi);

    direction.x = sinTheta*sinPhi;
    direction.y = -cosTheta;
    direction.z = sinTheta*cosPhi;
}

double SunPillbox::getIrradiance(void) const
{
	return irradiance.getValue();
}

double SunPillbox::getThetaMax() const
{
	return thetaMax.getValue();
}

SoNode* SunPillbox::copy(SbBool copyConnections) const
{
	// Use the standard version of the copy method to create
	// a copy of this instance, including its field data
    SunPillbox* sun = dynamic_cast<SunPillbox*>(SoNode::copy(copyConnections) );

    sun->irradiance = irradiance;
    sun->thetaMax = thetaMax;

    return sun;
}
