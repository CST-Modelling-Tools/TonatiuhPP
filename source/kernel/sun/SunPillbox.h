#pragma once

#include "kernel/sun/SunShape.h"


class TONATIUH_KERNEL SunPillbox: public SunShape
{
    SO_NODE_HEADER(SunPillbox);

public:
    SunPillbox();
    static void initClass();
    SoNode* copy(SbBool copyConnections) const;

    Vector3D generateRay(Random& rand) const;
    double getThetaMax() const;

    SoSFDouble thetaMax;

    NAME_ICON_FUNCTIONS("Pillbox", ":/images/SunPillbox.png")

protected:
    ~SunPillbox();

    double m_sinThetaMax;

    SoNodeSensor* m_sensor;
    static void onSensor(void* data, SoSensor*);
};
