#pragma once

#include "kernel/apertures/Aperture.h"


class TONATIUH_KERNEL ApertureStripe: public Aperture
{
    SO_NODE_HEADER(ApertureStripe);

public:
    static void initClass();
    ApertureStripe();

    SoSFDouble uMin;
    SoSFDouble uMax;
    SoSFDouble vMin;
    SoSFDouble vMax;

    BoundingBox getBox() const;
    bool isInside(double u, double v) const;
    QVector<Vector2D> makeMesh(const QSize& dims) const;

    NAME_ICON_FUNCTIONS("Strip", ":/images/ApertureStripe.png")
};
