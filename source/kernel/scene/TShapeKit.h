#pragma once

#include "kernel/TonatiuhKernel.h"
#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/fields/SoSFNode.h>

class ShapeRT;
class MaterialRT;

class SoFieldSensor;
class SoSensor;

//!  TShapeKit class groups what is necessary to the shape.
/*!
  TShapeKit groups the shape geometry, material and the transformation.

*/

class TONATIUH_KERNEL TShapeKit: public SoShapeKit
{
    SO_KIT_HEADER(TShapeKit);

public:
    static void initClass();
    TShapeKit();

    SoSFNode aperture;
    SoSFNode materialRT;

protected:
    virtual ~TShapeKit();

    SoFieldSensor* m_sensor;
    static void onSensor(void* data, SoSensor*);
};
