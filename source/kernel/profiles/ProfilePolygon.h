#pragma once

#include "kernel/profiles/ProfileRT.h"
#include "libraries/geometry/Vector2D.h"


class TONATIUH_KERNEL ProfilePolygon: public ProfileRT
{
    SO_NODE_HEADER(ProfilePolygon);

public:
    static void initClass();
    ProfilePolygon();

    SoSFVec2f a;
    SoSFVec2f b;
    SoSFVec2f c;

    Box3D getBox() const;
    bool isInside(double u, double v) const;
    QVector<Vector2D> makeMesh(const QSize& dims) const;

    NAME_ICON_FUNCTIONS("Polygon", ":/images/ProfilePolygon.png")

protected:
    ~ProfilePolygon();

    Vector2D m_pAC;
    Vector2D m_pBC;
    Vector2D m_pC;
    double m_det;

    SoNodeSensor* m_sensor;
    static void onSensor(void* data, SoSensor*);
};
