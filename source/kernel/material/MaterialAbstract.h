#pragma once

#include "kernel/TonatiuhKernel.h"
#include "kernel/scene/TAbstract.h"
#include <Inventor/nodes/SoMaterial.h>

struct DifferentialGeometry;
class RandomAbstract;
class Ray;


class TONATIUH_KERNEL MaterialAbstract: public SoMaterial
{
    SO_NODE_ABSTRACT_HEADER(MaterialAbstract);

public:
    static void initClass();

    //Ray* OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand) const;
    virtual bool OutputRay(const Ray& incident, DifferentialGeometry* dg, RandomAbstract& rand, Ray* outputRay) const = 0;

    NAME_ICON_FUNCTIONS("X", ":/MaterialX.png")

protected:
    MaterialAbstract() {}
    ~MaterialAbstract() {}
};
