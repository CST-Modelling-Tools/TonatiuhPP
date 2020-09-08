#pragma once

#include "kernel/material/MaterialRT.h"


class TONATIUH_KERNEL MaterialAbsorber: public MaterialRT
{
    SO_NODE_HEADER(MaterialAbsorber);

public:
    static void initClass();
    MaterialAbsorber();

    bool OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& rand, Ray& rayOut) const;

    NAME_ICON_FUNCTIONS("Absorber", ":/material/MaterialAbsorber.png")

protected:
    ~MaterialAbsorber() {}
};
