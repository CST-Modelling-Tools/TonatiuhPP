#pragma once

#include "kernel/TonatiuhKernel.h"
#include <Inventor/nodekits/SoShapeKit.h>
#include <QString>

class Random;
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

protected:
    virtual ~TShapeKit();
};
