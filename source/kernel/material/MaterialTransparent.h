#pragma once

#include "kernel/material/MaterialRT.h"


class TONATIUH_KERNEL MaterialTransparent: public MaterialRT
{
    SO_NODE_HEADER(MaterialTransparent);

public:
    static void initClass();
    MaterialTransparent();

    bool OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& rand, Ray& rayOut) const;

    NAME_ICON_FUNCTIONS("Transparent", ":/images/MaterialTransparent.png")

protected:
    ~MaterialTransparent() {}
};
