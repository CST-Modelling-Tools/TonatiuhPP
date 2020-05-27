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

public:
    TShapeKit();
    static void initClass();

    SbBool setPart(const SbName& part, SoNode* node);
    ShapeRT* getShape() {return m_shape;}
    MaterialRT* getMaterial() {return m_material;}
    ShapeRT* m_shape;
    MaterialRT* m_material;

protected:
    virtual ~TShapeKit();
};
