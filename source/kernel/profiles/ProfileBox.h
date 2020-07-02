#pragma once

#include "kernel/profiles/ProfileRT.h"


class TONATIUH_KERNEL ProfileBox: public ProfileRT
{
    SO_NODE_HEADER(ProfileBox);

public:
    static void initClass();
    ProfileBox();

    SoSFDouble uSize;
    SoSFDouble vSize;

    Box2D getBox() const;
    bool isInside(double u, double v) const;
    QVector<vec2d> makeMesh(QSize& dims) const;

    NAME_ICON_FUNCTIONS("Box", ":/images/ProfileBox.png")
};
