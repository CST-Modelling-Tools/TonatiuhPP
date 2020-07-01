#include "SunPillbox.h"

#include <Inventor/sensors/SoNodeSensor.h>

#include "libraries/math/gcf.h"

SO_NODE_SOURCE(SunPillbox)


void SunPillbox::initClass()
{
    SO_NODE_INIT_CLASS(SunPillbox, SunShape, "SunShape");
}

SunPillbox::SunPillbox()
{
    SO_NODE_CONSTRUCTOR(SunPillbox);
    SO_NODE_ADD_FIELD( thetaMax, (4.65e-3) );
    onSensor(this, 0);

    m_sensor = new SoNodeSensor(onSensor, this);
    m_sensor->attach(this);
}

SunPillbox::~SunPillbox()
{
    delete m_sensor;
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
    SunPillbox* sun = dynamic_cast<SunPillbox*>(SoNode::copy(copyConnections));

    sun->m_sinThetaMax = m_sinThetaMax; //?

    return sun;
}

void SunPillbox::onSensor(void* data, SoSensor*)
{
    SunPillbox* sun = (SunPillbox*) data;
    sun->m_sinThetaMax = sin(sun->thetaMax.getValue());
}
