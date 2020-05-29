#pragma once

#include "kernel/apertures/Aperture.h"


class TONATIUH_KERNEL ApertureRectangle: public Aperture
{
    SO_NODE_HEADER(ApertureRectangle);

public:
    static void initClass();
    ApertureRectangle();

    SoSFDouble sizeX;
    SoSFDouble sizeY;

    bool isInside(double x, double y) const;
    // mesh

    NAME_ICON_FUNCTIONS("Rectangle", ":/images/ApertureRectangle.png")

protected:
    ~ApertureRectangle() {}
};
