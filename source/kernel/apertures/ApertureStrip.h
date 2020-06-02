#pragma once

#include "kernel/apertures/Aperture.h"


class TONATIUH_KERNEL ApertureStrip: public Aperture
{
    SO_NODE_HEADER(ApertureStrip);

public:
    static void initClass();
    ApertureStrip();

    SoSFDouble uMin;
    SoSFDouble uMax;
    SoSFDouble vMin;
    SoSFDouble vMax;

    BoundingBox getBox() const;
    bool isInside(double u, double v) const;
    QVector<Vector2D> makeMesh(const QSize& dims) const;

    NAME_ICON_FUNCTIONS("Strip", ":/images/ApertureStrip.png")
};
