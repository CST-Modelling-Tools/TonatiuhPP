#pragma once

#include "kernel/profiles/ProfileRT.h"
#include "libraries/math/2D/vec2d.h"


class TONATIUH_KERNEL ProfileTriangle: public ProfileRT
{
    SO_NODE_HEADER(ProfileTriangle);

public:
    static void initClass();
    ProfileTriangle();

    SoSFVec2f a;
    SoSFVec2f b;
    SoSFVec2f c;

    Box2D getBox() const;
    bool isInside(double u, double v) const;
    QVector<vec2d> makeMesh(QSize& dims) const;

    NAME_ICON_FUNCTIONS("Triangle", ":/images/ProfileTriangle.png")

protected:
    ~ProfileTriangle();

    vec2d m_pAC;
    vec2d m_pBC;
    vec2d m_pC;
    double m_det;

    SoNodeSensor* m_sensor;
    static void onSensor(void* data, SoSensor*);
};
