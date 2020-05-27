#pragma once

#include "kernel/TonatiuhKernel.h"
#include <Inventor/nodekits/SoShapeKit.h>
#include <QString>

class Random;
class Ray;
class ShapeRT;
class MaterialRT;

//!  TShapeKit class groups what is necessary to the shape.
/*!
  TShapeKit groups the shape geometry, material and the transformation.

*/

class TONATIUH_KERNEL TShapeKit: public SoShapeKit
{
    SO_KIT_HEADER(TShapeKit);
    SO_KIT_CATALOG_ENTRY_HEADER(aperture);
    SO_KIT_CATALOG_ENTRY_HEADER(materialRT);

public:
    static void initClass();
    TShapeKit();

protected:
    virtual ~TShapeKit();
};
