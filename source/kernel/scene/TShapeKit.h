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

    SoSFNode shapeRT;
    SoSFNode profileRT;
    SoSFNode materialRT;

protected:
    virtual ~TShapeKit();

    void setDefaultOnNonWritingFields();

    SoFieldSensor* m_sensorShape;
    SoFieldSensor* m_sensorProfile;
    static void onSensor(void* data, SoSensor*);
};
