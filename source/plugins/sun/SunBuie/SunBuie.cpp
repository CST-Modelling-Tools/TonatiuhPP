#include "SunBuie.h"

#include <Inventor/sensors/SoFieldSensor.h>
#include "libraries/geometry/gc.h"


const double SunBuie::m_minCRSValue = 0.000001;
const double SunBuie::m_maxCRSValue = 0.849;


SO_NODE_SOURCE(SunBuie)

void SunBuie::initClass()
{
    SO_NODE_INIT_CLASS(SunBuie, SunShape, "SunShape");
}

SunBuie::SunBuie()
{
    SO_NODE_CONSTRUCTOR( SunBuie );
    SO_NODE_ADD_FIELD( irradiance, (1000.) );
    SO_NODE_ADD_FIELD( csr, (0.02) );

    m_sensorCSR = new SoFieldSensor(updateCSR, this);
    m_sensorCSR->attach(&csr);

	double csrValue = csr.getValue();
    if (csrValue >= m_minCRSValue && csrValue <= m_maxCRSValue)
        updateState(csrValue);
}

void SunBuie::updateState(double csrValue)
{
    m_thetaSD = 4.65e-3;
    m_thetaCS = 43.6e-3;
    m_deltaThetaCSSD = m_thetaCS - m_thetaSD;

    m_integralA = 9.224724736098827e-6;
    m_chi = chiValue(csrValue);
    m_k = 0.9*log(13.5*m_chi)*pow(m_chi, -0.3);
    m_gamma = 2.2*log(0.52*m_chi)*pow(m_chi, 0.43) - 0.1;
    m_etokTimes1000toGamma = exp(m_k)*pow(1000, m_gamma);

    double gammaPlusTwo = m_gamma + 2.;
    m_integralB = (exp(m_k)*pow(1000, m_gamma)/gammaPlusTwo) * ( pow(m_thetaCS, gammaPlusTwo) - pow(m_thetaSD, gammaPlusTwo) );

    m_alpha = 1./(m_integralA + m_integralB);
    m_heightRectangle1 = 1.001 * pdfTheta(0.0038915695846209047);
    m_heightRectangle2 = pdfTheta(m_thetaSD);

    double areaR1 = m_thetaSD*m_heightRectangle1;
    double areaR2 = m_deltaThetaCSSD*m_heightRectangle2;
    m_probabilityRectangle1 = areaR1/(areaR1 + areaR2);
}

SunBuie::~SunBuie()
{
    delete m_sensorCSR;
}

void SunBuie::GenerateRayDirection(Vector3D& direction, RandomDeviate& rand) const
{
    double phi = gc::TwoPi*rand.RandomDouble();
    double theta = zenithAngle(rand);
    double sinTheta = sin(theta);
    double cosTheta = cos(theta);
    double cosPhi = cos(phi);
    double sinPhi = sin(phi);

    direction.x = sinTheta*sinPhi;
    direction.y = -cosTheta;
    direction.z = sinTheta*cosPhi;
}

double SunBuie::GetIrradiance() const
{
	return irradiance.getValue();
}

double SunBuie::GetThetaMax() const
{
    return m_thetaCS;
}

SoNode* SunBuie::copy(SbBool copyConnections) const
{
	// Use the standard version of the copy method to create
	// a copy of this instance, including its field data
    SunBuie* newSunShape = dynamic_cast<SunBuie*>( SoNode::copy(copyConnections) );

	newSunShape->irradiance = irradiance;
	newSunShape->csr = csr;

	newSunShape->m_chi = m_chi;
	newSunShape->m_k = m_k;
	newSunShape->m_gamma = m_gamma;
	newSunShape->m_etokTimes1000toGamma = m_etokTimes1000toGamma;
	newSunShape->m_thetaSD = m_thetaSD;
	newSunShape->m_thetaCS = m_thetaCS;
	newSunShape->m_deltaThetaCSSD = m_deltaThetaCSSD;
	newSunShape->m_integralA = m_integralA;
	newSunShape->m_integralB = m_integralB;
	newSunShape->m_alpha = m_alpha;
	newSunShape->m_heightRectangle1 = m_heightRectangle1;
	newSunShape->m_heightRectangle2 = m_heightRectangle2;
	newSunShape->m_probabilityRectangle1 = m_probabilityRectangle1;

	return newSunShape;
}

void SunBuie::updateCSR(void* data, SoSensor*)
{
    SunBuie* sunshape = (SunBuie*) data;
	double csrValue = sunshape->csr.getValue();
    if (csrValue >= m_minCRSValue && csrValue <= m_maxCRSValue)
        sunshape->updateState(csrValue);
}

double SunBuie::zenithAngle(RandomDeviate& rand) const
{
	double theta;
	double value;

    do {
        if (rand.RandomDouble() < m_probabilityRectangle1) {
            theta = rand.RandomDouble()*m_thetaSD;
            value = rand.RandomDouble()*m_heightRectangle1;
        } else {
            theta = m_thetaSD + rand.RandomDouble()*m_deltaThetaCSSD;
            value = rand.RandomDouble()*m_heightRectangle2;
		}
    } while (value > pdfTheta(theta));

	return theta;
}

double SunBuie::chiValue(double csr) const
{
    if (csr > 0.145)
        return -0.04419909985804843 + csr*(
                1.401323894233574 + csr*(
                -0.3639746714505299 + csr*(
                -0.9579768560161194 + 1.1550475450828657*csr)));
    else if (csr > 0.035)
        return 0.022652077593662934 + csr*(
               0.5252380349996234 + csr*(
               2.5484334534423887 - 0.8763755326550412*csr));
    else
        return 0.004733749294807862 + csr*(
               4.716738065192151 + csr*(
               -463.506669149804 + csr*(
               24745.88727411664 + csr*(
               -606122.7511711778 + 5521693.445014727*csr))));
}

double SunBuie::phi(double theta) const
{
    if (theta < m_thetaSD)
        return cos(326.*theta)/cos(308.*theta);
    else
        return m_etokTimes1000toGamma*pow(theta, m_gamma);
}

double SunBuie::pdfTheta(double theta) const
{
    return m_alpha*phi(theta)*sin(theta);
}

