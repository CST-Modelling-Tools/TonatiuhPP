#pragma once

#include "kernel/material/MaterialRT.h"
#include "kernel/TonatiuhTypes.h"


class TONATIUH_KERNEL MaterialVirtual: public MaterialRT
{
	SO_NODE_HEADER(MaterialVirtual);

public:
    static void initClass();
    MaterialVirtual();

    bool OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& rand, Ray& rayOut) const;

    NAME_ICON_FUNCTIONS("Virtual", ":/images/MaterialVirtual.png")

protected:
    ~MaterialVirtual() {}
};
