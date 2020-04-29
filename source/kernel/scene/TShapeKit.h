#pragma once

#include "kernel/TonatiuhKernel.h"
#include <Inventor/nodekits/SoShapeKit.h>
#include <QString>

class RandomAbstract;
class Ray;

//!  TShapeKit class groups what is necessary to the shape.
/*!
  TShapeKit groups the shape geometry, material and the transformation.

*/

class TONATIUH_KERNEL TShapeKit : public SoShapeKit
{
    typedef SoShapeKit inherited;

    SO_KIT_HEADER(TShapeKit);

public:
    TShapeKit();
    static void initClass();

    bool IntersectP(const Ray& ray) const;
    Ray* Intersect(const Ray& objectRay, bool* isShapeFront, RandomAbstract& rand) const;

protected:
    virtual ~TShapeKit();
};
