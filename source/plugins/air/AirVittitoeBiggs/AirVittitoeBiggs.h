#pragma once

#include <Inventor/fields/SoSFEnum.h>

#include "kernel/raytracing/trt.h"
#include "kernel/raytracing/TTransmissivity.h"


class AirVittitoeBiggs: public TTransmissivity
{
    SO_NODE_HEADER(AirVittitoeBiggs);

public:
    static void initClass();
    AirVittitoeBiggs();

    double transmission(double distance) const;

    enum Visibility {ClearDay, HazyDay};
    SoSFEnum visibility;

    static const char* getClassName() {return "VittitoeBiggs";}
};



#include "kernel/raytracing/TTransmissivityFactory.h"

class AirVittitoeBiggsFactory: public QObject, public AirFactory<AirVittitoeBiggs>
{
    Q_OBJECT
    Q_INTERFACES(TTransmissivityFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.TTransmissivityFactory")
};
