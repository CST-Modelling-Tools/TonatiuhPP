#include "SunPillbox.h"

#include "libraries/geometry/gcf.h"
#include <Inventor/sensors/SoFieldSensor.h>

SO_NODE_SOURCE(SunPillbox)


void SunPillbox::initClass()
{
    SO_NODE_INIT_CLASS(SunPillbox, SunShape, "SunAbstract");
}

SunPillbox::SunPillbox()
{
    SO_NODE_CONSTRUCTOR(SunPillbox);
    SO_NODE_ADD_FIELD( thetaMax, (4.65e-3) );
    updateTheta(this, 0);

    m_sensor_thetaMax = new SoFieldSensor(updateTheta, this);
    m_sensor_thetaMax->attach(&thetaMax);
}

SunPillbox::~SunPillbox()
{
    delete m_sensor_thetaMax;
}

void SunPillbox::updateTheta(void* data, SoSensor*)
{
    SunPillbox* sun = (SunPillbox*) data;
    sun->m_sinThetaMax = sin(sun->thetaMax.getValue());
}

Vector3D SunPillbox::generateRay(Random& rand) const
{
    double phi = gcf::TwoPi*rand.RandomDouble();
    double sinTheta = m_sinThetaMax*sqrt(rand.RandomDouble());
    double cosTheta = sqrt(1. - sinTheta*sinTheta);
    double cosPhi = cos(phi);
    double sinPhi = sin(phi);

    return Vector3D(
        sinTheta*cosPhi,
        sinTheta*sinPhi,
        cosTheta
    );
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

    sun->thetaMax = thetaMax;

    return sun;
}
