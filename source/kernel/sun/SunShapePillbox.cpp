#include "SunShapePillbox.h"

#include <Inventor/sensors/SoNodeSensor.h>

#include "libraries/math/gcf.h"

SO_NODE_SOURCE(SunShapePillbox)


void SunShapePillbox::initClass()
{
    SO_NODE_INIT_CLASS(SunShapePillbox, SunShape, "SunShape");
}

SunShapePillbox::SunShapePillbox()
{
    SO_NODE_CONSTRUCTOR(SunShapePillbox);
    SO_NODE_ADD_FIELD( thetaMax, (4.65e-3) );
    onSensor(this, 0);

    m_sensor = new SoNodeSensor(onSensor, this);
    m_sensor->setPriority(0);
    m_sensor->attach(this);
}

SunShapePillbox::~SunShapePillbox()
{
    delete m_sensor;
}

vec3d SunShapePillbox::generateRay(Random& rand) const
{
    double phi = gcf::TwoPi*rand.RandomDouble();
    double sinTheta = m_sinThetaMax*sqrt(rand.RandomDouble());
    double cosTheta = sqrt(1. - sinTheta*sinTheta);
    double cosPhi = cos(phi);
    double sinPhi = sin(phi);

    return vec3d(
        sinTheta*cosPhi,
        sinTheta*sinPhi,
        cosTheta
    );
}

double SunShapePillbox::getThetaMax() const
{
    return thetaMax.getValue();
}

double SunShapePillbox::shape(double theta) const
{
    if (std::abs(theta) > thetaMax.getValue())
        return 0;
    return 1.;
}

SoNode* SunShapePillbox::copy(SbBool copyConnections) const
{
    // Use the standard version of the copy method to create
    // a copy of this instance, including its field data
    SunShapePillbox* sun = dynamic_cast<SunShapePillbox*>(SoNode::copy(copyConnections));
    sun->m_sinThetaMax = m_sinThetaMax;
    return sun;
}

void SunShapePillbox::onSensor(void* data, SoSensor*)
{
    SunShapePillbox* sun = (SunShapePillbox*) data;
    sun->m_sinThetaMax = sin(sun->thetaMax.getValue());
}
