#pragma once

#include "kernel/TonatiuhKernel.h"
#include "kernel/raytracing/TAbstract.h"
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFDouble.h>

#include "libraries/geometry/Vector3D.h"
#include "kernel/random/RandomDeviate.h"


class TONATIUH_KERNEL SunAbstract: public SoNode
{
    typedef SoNode inherited;

    SO_NODE_ABSTRACT_HEADER(SunAbstract);

public:
    static void initClass();

    virtual void GenerateRayDirection(Vector3D& direction, RandomDeviate& rand) const = 0;
    virtual double GetIrradiance() const = 0;
    virtual double GetThetaMax() const = 0;

    NAME_ICON_FUNCTIONS("X", ":/SunX.png")

protected:
    SunAbstract() {}
    virtual ~SunAbstract() {}
};
