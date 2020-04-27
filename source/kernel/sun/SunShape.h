#pragma once

#include "kernel/TonatiuhKernel.h"
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFDouble.h>

#include "libraries/geometry/Vector3D.h"
#include "kernel/random/RandomDeviate.h"


class TONATIUH_KERNEL SunShape: public SoNode
{
    typedef SoNode inherited;

    SO_NODE_ABSTRACT_HEADER(SunShape);

public:
    static void initClass();

    virtual void GenerateRayDirection(Vector3D& direction, RandomDeviate& rand) const = 0;
    virtual double GetIrradiance() const = 0;
    virtual double GetThetaMax() const = 0;

    static const char* getClassName() {return "SunShape";}

protected:
    SunShape() {}
    virtual ~SunShape() {}
};
