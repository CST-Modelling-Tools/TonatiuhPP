#pragma once

#include <Inventor/fields/SoSFDouble.h>
#include "kernel/sun/SunAbstract.h"

class SoSensor;
class SoFieldSensor;


class TONATIUH_KERNEL SunPillbox: public SunAbstract
{
    SO_NODE_HEADER(SunPillbox);

public:
    SunPillbox();
    static void initClass();
    SoNode* copy(SbBool copyConnections) const;

    void generateRay(Vector3D& direction, RandomAbstract& rand) const;
    double getThetaMax() const;

    SoSFDouble thetaMax;

    NAME_ICON_FUNCTIONS("Pillbox", ":/images/SunPillbox.png")

protected:
    ~SunPillbox();
    static void updateTheta(void* data, SoSensor*);

private:
    SoFieldSensor* m_sensor_thetaMax;
    double m_sinThetaMax;
};
