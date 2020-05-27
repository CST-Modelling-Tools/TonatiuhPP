#pragma once

#include "kernel/apertures/Aperture.h"


class TONATIUH_KERNEL ApertureRectangle: public Aperture
{
    SO_NODE_HEADER(ApertureRectangle);

public:
    static void initClass();
    ApertureRectangle();

    bool OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& rand, Ray& rayOut) const;

    NAME_ICON_FUNCTIONS("Rectangle", ":/images/ApertureRectangle.png")

protected:
    ~ApertureRectangle() {}
};
