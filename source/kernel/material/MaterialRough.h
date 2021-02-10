#pragma once

#include "kernel/material/MaterialRT.h"


class TONATIUH_KERNEL MaterialRough: public MaterialRT
{
    SO_NODE_HEADER(MaterialRough);

public:
    enum Distribution {
        Beckmann, // Beckmann–Spizzichino, similar to Gaussian
        Trowbridge // Trowbridge–Reitz, similar to Lorentzian
    };

    static void initClass();
    MaterialRough();

    bool OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& rand, Ray& rayOut) const;

    SoSFDouble diffuse;
    SoSFDouble specular;
    SoSFEnum distribution;
    SoSFDouble roughness;

    NAME_ICON_FUNCTIONS("Rough", ":/material/MaterialRough.png")

protected:
    ~MaterialRough() {}
};

