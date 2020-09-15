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

    SoFieldSensor* m_sensorShape;
    SoFieldSensor* m_sensorProfile;
    static void onSensor(void* data, SoSensor*);

    void setDefaultOnNonWritingFields();
    SbBool setUpConnections(SbBool onoff, SbBool doitalways);
};
