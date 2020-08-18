#pragma once

#include "kernel/air/AirTransmission.h"


class TONATIUH_KERNEL AirPolynomial: public AirTransmission
{
    SO_NODE_HEADER(AirPolynomial);

public:
    enum Coefficients {
        Custom,
        VittitoeBiggs_clear,
        VittitoeBiggs_hazy,
        Ballestrin_clear,
        Ballestrin_hazy
    };

    static void initClass();
    AirPolynomial();
    ~AirPolynomial();

    double transmission(double distance) const;

    SoSFEnum coefficients;

    SoSFDouble a0;
    SoSFDouble a1;
    SoSFDouble a2;
    SoSFDouble a3;

    NAME_ICON_FUNCTIONS("Polynomial", ":/images/AirAbstract.png")

protected:
    static void update_coefficients(void* data, SoSensor*);

private:
    SoFieldSensor* m_sensor_coefficients;
};
