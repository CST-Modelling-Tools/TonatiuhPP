#pragma once

#include "kernel/apertures/Aperture.h"


class TONATIUH_KERNEL ApertureStrip: public Aperture
{
    SO_NODE_HEADER(ApertureStrip);

public:
    static void initClass();
    ApertureStrip();

    SoSFDouble sizeX;
    SoSFDouble sizeY;

    bool isInside(double x, double y) const;
    // mesh

    NAME_ICON_FUNCTIONS("Strip", ":/images/ApertureStrip.png")

protected:
    ~ApertureStrip() {}
};
