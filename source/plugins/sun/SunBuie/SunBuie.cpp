#include "SunBuie.h"

#include <Inventor/sensors/SoFieldSensor.h>
#include "libraries/geometry/gcf.h"


const double SunBuie::m_minCRSValue = 0.000001;
const double SunBuie::m_maxCRSValue = 0.849;


SO_NODE_SOURCE(SunBuie)

void SunBuie::initClass()
{
    SO_NODE_INIT_CLASS(SunBuie, SunAbstract, "SunAbstract");
}

SunBuie::SunBuie()
{
    SO_NODE_CONSTRUCTOR(SunBuie);
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

    m_chi = chiValue(csrValue);
    m_k = 0.9*log(13.5*m_chi)*pow(m_chi, -0.3);
    m_gamma = 2.2*log(0.52*m_chi)*pow(m_chi, 0.43) - 0.1;
    m_exp = exp(m_k)*pow(1000, m_gamma);

    double m_integralA = 9.224724736098827e-6;
    double gammaPlusTwo = m_gamma + 2.;
    double m_integralB = (exp(m_k)*pow(1000, m_gamma)/gammaPlusTwo) * ( pow(m_thetaCS, gammaPlusTwo) - pow(m_thetaSD, gammaPlusTwo) );
    m_alpha = 1./(m_integralA + m_integralB);

    m_heightSD = 1.001 * pdfTheta(0.0038915695846209047);
    m_heightCS = pdfTheta(m_thetaSD);

    double areaA = m_thetaSD*m_heightSD;
    double areaB = m_deltaThetaCSSD*m_heightCS;
    m_probabilitySD = areaA/(areaA + areaB);
}

SunBuie::~SunBuie()
{
    delete m_sensorCSR;
}

void SunBuie::generateRay(Vector3D& direction, RandomAbstract& rand) const
{
    double phi = gcf::TwoPi*rand.RandomDouble();
    double theta = zenithAngle(rand);
    double sinTheta = sin(theta);
    double cosTheta = cos(theta);
    double cosPhi = cos(phi);
    double sinPhi = sin(phi);

    direction.x = sinTheta*cosPhi;
    direction.y = sinTheta*sinPhi;
    direction.z = cosTheta;
}

double SunBuie::getIrradiance() const
{
	return irradiance.getValue();
}

double SunBuie::getThetaMax() const
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
    newSunShape->m_exp = m_exp;
	newSunShape->m_thetaSD = m_thetaSD;
	newSunShape->m_thetaCS = m_thetaCS;
	newSunShape->m_deltaThetaCSSD = m_deltaThetaCSSD;
	newSunShape->m_alpha = m_alpha;
    newSunShape->m_heightSD = m_heightSD;
    newSunShape->m_heightCS = m_heightCS;
    newSunShape->m_probabilitySD = m_probabilitySD;

	return newSunShape;
}

void SunBuie::updateCSR(void* data, SoSensor*)
{
    SunBuie* sunshape = (SunBuie*) data;
	double csrValue = sunshape->csr.getValue();
    if (csrValue >= m_minCRSValue && csrValue <= m_maxCRSValue)
        sunshape->updateState(csrValue);
}

double SunBuie::zenithAngle(RandomAbstract& rand) const
{
	double theta;
	double value;

    do {
        if (rand.RandomDouble() < m_probabilitySD) {
            theta = rand.RandomDouble()*m_thetaSD;
            value = rand.RandomDouble()*m_heightSD;
        } else {
            theta = m_thetaSD + rand.RandomDouble()*m_deltaThetaCSSD;
            value = rand.RandomDouble()*m_heightCS;
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
        return m_exp*pow(theta, m_gamma);
}

double SunBuie::pdfTheta(double theta) const
{
    return m_alpha*phi(theta)*sin(theta);
}


// References:
// D. Buie, C.J. Dey, S. Bosi. "The effective size of the solar cone for solar concentrating systems". Solar Energy 74 (2003) 417�427.
// D. Buie, A.G. Monger, C.J. Dey. "Sunshape distributions for terrestrial solar simulations". Solar Energy 74 (2003) 113�122.
