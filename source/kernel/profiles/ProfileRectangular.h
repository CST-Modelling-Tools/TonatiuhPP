#pragma once

#include "kernel/profiles/ProfileRT.h"


class TONATIUH_KERNEL ProfileRectangular: public ProfileRT
{
    SO_NODE_HEADER(ProfileRectangular);

public:
    static void initClass();
    ProfileRectangular();

    SoSFDouble uMin;
    SoSFDouble uMax;
    SoSFDouble vMin;
    SoSFDouble vMax;

    Box3D getBox() const;
    bool isInside(double u, double v) const;
    QVector<vec2d> makeMesh(QSize& dims) const;

    NAME_ICON_FUNCTIONS("Rectangular", ":/images/ProfileRectangular.png")
};
