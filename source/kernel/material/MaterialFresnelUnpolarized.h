#pragma once

#include "kernel/material/MaterialRT.h"


class TONATIUH_KERNEL MaterialFresnelUnpolarized: public MaterialRT
{
    SO_NODE_HEADER(MaterialFresnelUnpolarized);

public:
    enum Distribution {
        pillbox,
        Gaussian
    };

    static void initClass();
    MaterialFresnelUnpolarized();

    SoSFDouble nFront;
    SoSFDouble nBack;

    SoSFEnum distribution;
    SoSFDouble slope;

    bool OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& rand, Ray& rayOut) const;

    NAME_ICON_FUNCTIONS("Fresnel (unpolarized)", ":/material/MaterialFresnel.png")

protected:
    ~MaterialFresnelUnpolarized() {}
};

