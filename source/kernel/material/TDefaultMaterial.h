#pragma once

#include "MaterialAbstract.h"


class TONATIUH_KERNEL TDefaultMaterial: public MaterialAbstract
{
    SO_NODE_HEADER(TDefaultMaterial);
    
public:
    static void initClass();
    TDefaultMaterial();
    
    bool OutputRay(const Ray& incident, DifferentialGeometry* dg, RandomAbstract& rand, Ray* outputRay) const;
};
