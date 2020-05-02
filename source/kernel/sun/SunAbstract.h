#pragma once

#include "kernel/scene/TAbstract.h"
#include "kernel/random/RandomAbstract.h"
#include "libraries/geometry/Vector3D.h"

#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFDouble.h>


class TONATIUH_KERNEL SunAbstract: public SoNode
{
    typedef SoNode inherited;

    SO_NODE_ABSTRACT_HEADER(SunAbstract);

public:
    static void initClass();

    virtual void generateRay(Vector3D& direction, RandomAbstract& rand) const = 0;
    virtual double getIrradiance() const = 0;
    virtual double getThetaMax() const = 0;

    NAME_ICON_FUNCTIONS("X", ":/SunX.png")

protected:
    SunAbstract() {}
    ~SunAbstract() {}
};
