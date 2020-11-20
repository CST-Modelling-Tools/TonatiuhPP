#pragma once

#include <QSharedPointer>
#include <Inventor/fields/SoMFVec2f.h>

#include "kernel/profiles/ProfileRT.h"
#include "libraries/math/2D/vec2d.h"
#include "libraries/Coin3D/MFVec2.h"


class TONATIUH_KERNEL ProfilePolygon: public ProfileRT
{
    SO_NODE_HEADER(ProfilePolygon);

public:
    static void initClass();
    ProfilePolygon();

    SoMFVec2f points;
    const QPolygonF& getPolygon() const {return m_polygon;}

    Box2D getBox() const;
    void setBox(const Box2D& box);

    bool isInside(double u, double v) const;
    QVector<vec2d> makeMesh(QSize& dims) const;

    NAME_ICON_FUNCTIONS("Polygon", ":/profiles/ProfilePolygon.png")

protected:
    QPolygonF m_polygon;
    Box2D m_box;

    QSharedPointer<SoFieldSensor> m_sensor;
    static void onSensor(void* data, SoSensor*);
};
