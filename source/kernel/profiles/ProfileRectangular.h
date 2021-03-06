#pragma once

#include "kernel/profiles/ProfileRT.h"


class TONATIUH_KERNEL ProfileRectangular: public ProfileRT
{
    SO_NODE_HEADER(ProfileRectangular);

public:
    static void initClass();
    ProfileRectangular();

    TSFDoubleUnit uMin;
    TSFDoubleUnit uMax;
    TSFDoubleUnit vMin;
    TSFDoubleUnit vMax;

    Box2D getBox() const;
    void setBox(const Box2D& box);

    bool isInside(double u, double v) const;
    QVector<vec2d> makeMesh(QSize& dims) const;

    NAME_ICON_FUNCTIONS("Rectangular", ":/profiles/ProfileRectangular.png")
};
