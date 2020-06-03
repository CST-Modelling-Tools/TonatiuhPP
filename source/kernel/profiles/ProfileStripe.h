#pragma once

#include "kernel/profiles/ProfileRT.h"


class TONATIUH_KERNEL ProfileStripe: public ProfileRT
{
    SO_NODE_HEADER(ProfileStripe);

public:
    static void initClass();
    ProfileStripe();

    SoSFDouble uMin;
    SoSFDouble uMax;
    SoSFDouble vMin;
    SoSFDouble vMax;

    BoundingBox getBox() const;
    bool isInside(double u, double v) const;
    QVector<Vector2D> makeMesh(const QSize& dims) const;

    NAME_ICON_FUNCTIONS("Strip", ":/images/ProfileStripe.png")
};
