#pragma once

#include "kernel/material/MaterialRT.h"


class TONATIUH_KERNEL MaterialFresnel: public MaterialRT
{
    SO_NODE_HEADER(MaterialFresnel);

public:
    static void initClass();
    MaterialFresnel();

    bool OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& rand, Ray& rayOut) const;

    NAME_ICON_FUNCTIONS("Fresnel", ":/material/MaterialFresnel.png")

protected:
    ~MaterialFresnel() {}
};

