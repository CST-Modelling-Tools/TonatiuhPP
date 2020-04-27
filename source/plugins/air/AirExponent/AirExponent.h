#pragma once

#include <Inventor/fields/SoSFDouble.h>

#include "kernel/air/AirAbstract.h"


class AirExponent: public AirAbstract
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



#include "kernel/air/AirFactory.h"

class AirExponentFactory:
    public QObject, public AirFactoryT<AirExponent>
{
    Q_OBJECT
    Q_INTERFACES(AirFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.AirFactory")
};
