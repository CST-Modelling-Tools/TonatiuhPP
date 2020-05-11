#pragma once

#include "kernel/material/MaterialAbstract.h"
#include "kernel/TonatiuhTypes.h"


class TONATIUH_KERNEL MaterialVirtual: public MaterialAbstract
{
	SO_NODE_HEADER(MaterialVirtual);

public:
    static void initClass();
    MaterialVirtual();

    bool OutputRay(const Ray& rayIn, DifferentialGeometry* dg, RandomAbstract& rand, Ray* rayOut) const;

    NAME_ICON_FUNCTIONS("Virtual", ":/images/MaterialVirtual.png")

protected:
    ~MaterialVirtual() {}
};
