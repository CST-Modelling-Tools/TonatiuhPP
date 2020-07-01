#pragma once

#include "kernel/profiles/ProfileRT.h"
#include "libraries/math/Vector2D.h"


class TONATIUH_KERNEL ProfileTriangle: public ProfileRT
{
    SO_NODE_HEADER(ProfileTriangle);

public:
    static void initClass();
    ProfileTriangle();

    SoSFVec2f a;
    SoSFVec2f b;
    SoSFVec2f c;

    Box3D getBox() const;
    bool isInside(double u, double v) const;
    QVector<Vector2D> makeMesh(QSize& dims) const;

    NAME_ICON_FUNCTIONS("Triangle", ":/images/ProfileTriangle.png")

protected:
    ~ProfileTriangle();

    Vector2D m_pAC;
    Vector2D m_pBC;
    Vector2D m_pC;
    double m_det;

    SoNodeSensor* m_sensor;
    static void onSensor(void* data, SoSensor*);
};
