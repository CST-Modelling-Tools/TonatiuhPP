#pragma once

#include "kernel/sun/SunShape.h"


class TONATIUH_KERNEL SunShapePillbox: public SunShape
{
    SO_NODE_HEADER(SunShapePillbox);

public:
    SunShapePillbox();
    static void initClass();
    SoNode* copy(SbBool copyConnections) const;

    vec3d generateRay(Random& rand) const;
    double getThetaMax() const;

    SoSFDouble thetaMax;

    NAME_ICON_FUNCTIONS("Pillbox", ":/images/SunPillbox.png")

protected:
    ~SunShapePillbox();

    double m_sinThetaMax;

    SoNodeSensor* m_sensor;
    static void onSensor(void* data, SoSensor*);
};
