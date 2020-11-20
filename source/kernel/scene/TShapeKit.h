#pragma once

#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodekits/SoShapeKit.h>

class SoFieldSensor;
class SoSensor;


class TONATIUH_KERNEL TShapeKit: public SoBaseKit
{
    SO_KIT_HEADER(TShapeKit);

    SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);

public:
    static void initClass();
    TShapeKit();
    TShapeKit* copy(SbBool copyConnections) const;

    SoSFNode shapeRT;
    SoSFNode profileRT;
    SoSFNode materialRT;
    SoSFNode material;

    SoShapeKit* m_shapeKit;

protected:
     ~TShapeKit();
public:
    SoFieldSensor* m_sensor_shapeRT;
    SoFieldSensor* m_sensor_profileRT;
    SoFieldSensor* m_sensor_material;

    static void onSensor(void* data, SoSensor*);
    static void onSensor_shapeRT(void* data, SoSensor*);

    void setDefaultOnNonWritingFields();
    SbBool setUpConnections(SbBool onoff, SbBool doitalways);

    virtual SbBool readInstance(SoInput * in, unsigned short flags);

    // SoNode interface
//public:
//    void rayPick(SoRayPickAction* action);
};
