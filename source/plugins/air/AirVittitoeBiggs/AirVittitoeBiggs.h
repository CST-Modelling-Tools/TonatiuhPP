#pragma once

#include <Inventor/fields/SoSFEnum.h>

#include "kernel/air/TTransmissivity.h"


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
    static const char* getClassIcon() {return ":/AirVittitoeBiggs.png";}
};



#include "kernel/air/TTransmissivityFactory.h"

class AirVittitoeBiggsFactory:
    public QObject, public AirFactory<AirVittitoeBiggs>
{
    Q_OBJECT
    Q_INTERFACES(TTransmissivityFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.TTransmissivityFactory")
};
