#pragma once

#include <Inventor/fields/SoSFDouble.h>

#include "kernel/air/TTransmissivity.h"


class AirExponent: public TTransmissivity
{
    SO_NODE_HEADER(AirExponent);

public:
    static void initClass();
    AirExponent();

    double transmission(double distance) const;
    SoSFDouble constant;

    static const char* getClassName() {return "Exponent";}
    static const char* getClassIcon() {return ":/AirExponent.png";}
};



#include "kernel/air/TTransmissivityFactory.h"

class AirExponentFactory:
    public QObject, public AirFactory<AirExponent>
{
    Q_OBJECT
    Q_INTERFACES(TTransmissivityFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.TTransmissivityFactory")
};
