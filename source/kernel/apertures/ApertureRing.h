#pragma once

#include "kernel/apertures/Aperture.h"


class TONATIUH_KERNEL ApertureRing: public Aperture
{
    SO_NODE_HEADER(ApertureRing);

public:
    static void initClass();
    ApertureRing();

    SoSFDouble sizeX;
    SoSFDouble sizeY;

    bool isInside(double x, double y) const;
    // mesh

    NAME_ICON_FUNCTIONS("Ring", ":/images/ApertureRing.png")

protected:
    ~ApertureRing() {}
};
