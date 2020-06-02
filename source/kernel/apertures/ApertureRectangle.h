#pragma once

#include "kernel/apertures/Aperture.h"


class TONATIUH_KERNEL ApertureRectangle: public Aperture
{
    SO_NODE_HEADER(ApertureRectangle);

public:
    static void initClass();
    ApertureRectangle();

    SoSFDouble uSize;
    SoSFDouble vSize;

    BoundingBox getBox() const;
    bool isInside(double u, double v) const;
    QVector<Vector2D> makeMesh(const QSize& dims) const;

    NAME_ICON_FUNCTIONS("Rectangle", ":/images/ApertureRectangle.png")
};
