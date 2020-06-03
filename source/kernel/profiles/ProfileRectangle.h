#pragma once

#include "kernel/profiles/ProfileRT.h"


class TONATIUH_KERNEL ProfileRectangle: public ProfileRT
{
    SO_NODE_HEADER(ProfileRectangle);

public:
    static void initClass();
    ProfileRectangle();

    SoSFDouble uSize;
    SoSFDouble vSize;

    BoundingBox getBox() const;
    bool isInside(double u, double v) const;
    QVector<Vector2D> makeMesh(const QSize& dims) const;

    NAME_ICON_FUNCTIONS("Rectangle", ":/images/ProfileRectangle.png")
};
