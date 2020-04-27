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

    void GenerateRayDirection(Vector3D& direction, RandomDeviate& rand) const;
	double GetIrradiance() const;
    double GetThetaMax() const;

    SoSFDouble irradiance;
    SoSFDouble thetaMax;

    static const char* getClassName() {return "Pillbox";}
    static const char* getClassIcon() {return ":/images/SunPillbox.png";}

protected:
    ~SunPillbox();
    static void updateTheta(void* data, SoSensor*);

private:
    SoFieldSensor* m_sensorTheta;
    double m_sinThetaMax;
};
