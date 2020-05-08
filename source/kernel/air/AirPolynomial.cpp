#include "AirPolynomial.h"

#include <Inventor/sensors/SoFieldSensor.h>

SO_NODE_SOURCE(AirPolynomial)


void AirPolynomial::initClass()
{
    SO_NODE_INIT_CLASS(AirPolynomial, AirAbstract, "AirAbstract");
}

AirPolynomial::AirPolynomial()
{
    SO_NODE_CONSTRUCTOR(AirPolynomial);
    setName("Polynomial");

    SO_NODE_DEFINE_ENUM_VALUE(Coefficients, Custom);
    SO_NODE_DEFINE_ENUM_VALUE(Coefficients, VittitoeBiggs_clear);
    SO_NODE_DEFINE_ENUM_VALUE(Coefficients, VittitoeBiggs_hazy);
    SO_NODE_DEFINE_ENUM_VALUE(Coefficients, Ballestrin_clear);
    SO_NODE_DEFINE_ENUM_VALUE(Coefficients, Ballestrin_hazy);

    SO_NODE_SET_SF_ENUM_TYPE(coefficients, Coefficients);
    SO_NODE_ADD_FIELD( coefficients, (Custom) );

    SO_NODE_ADD_FIELD(a0, (1.) );
    SO_NODE_ADD_FIELD(a1, (0.) );
    SO_NODE_ADD_FIELD(a2, (0.) );
    SO_NODE_ADD_FIELD(a3, (0.) );

    m_sensor_coefficients = new SoFieldSensor(update_coefficients, this);
    m_sensor_coefficients->attach(&coefficients);
}

AirPolynomial::~AirPolynomial()
{
    delete m_sensor_coefficients;
}

double AirPolynomial::transmission(double distance) const
{
    double d = distance/1000.;
    double t = a0.getValue() + a1.getValue()*d + a2.getValue()*d*d + a3.getValue()*d*d*d;
    return t;
}

void AirPolynomial::update_coefficients(void* data, SoSensor*)
{
    AirPolynomial* air = (AirPolynomial*) data;
    if (air->coefficients.getValue() == VittitoeBiggs_clear) {
        air->a0 = 0.99326;
        air->a1 = -0.1046;
        air->a2 = 0.017;
        air->a3 = -0.002845;
    } else if (air->coefficients.getValue() == VittitoeBiggs_hazy) {
        air->a0 = 0.98707;
        air->a1 = -0.2748;
        air->a2 = 0.03394;
        air->a3 = 0.;
    } else if (air->coefficients.getValue() == Ballestrin_clear) {
        air->a0 = 0.9970456;
        air->a1 = -0.1522128;
        air->a2 = 0.018598;
        air->a3 = -0.0015182;
    } else if (air->coefficients.getValue() == Ballestrin_hazy) {
        air->a0 = 0.9922059;
        air->a1 = -0.5549083;
        air->a2 = 0.147887;
        air->a3 = -0.0153718;
    }
}
