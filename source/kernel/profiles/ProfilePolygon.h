#pragma once

#include "kernel/profiles/ProfileRT.h"
#include "libraries/math/2D/vec2d.h"
#include "libraries/Coin3D/MFVec2.h"
#include <Inventor/fields/SoMFVec2f.h>

class TONATIUH_KERNEL ProfilePolygon: public ProfileRT
{
    SO_NODE_HEADER(ProfilePolygon);

public:
    static void initClass();
    ProfilePolygon();

    SoMFVec2f points;
    const QPolygonF& getPolygon() const {return m_polygon;}

    Box3D getBox() const;
    bool isInside(double u, double v) const;
    QVector<vec2d> makeMesh(QSize& dims) const;

    NAME_ICON_FUNCTIONS("Polygon", ":/images/ProfilePolygon.png")

protected:
    ~ProfilePolygon();

    QPolygonF m_polygon;

    SoNodeSensor* m_sensor;
    static void onSensor(void* data, SoSensor*);
};
