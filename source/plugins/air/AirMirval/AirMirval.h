#pragma once

#include "kernel/air/Air.h"


class AirMirval: public Air
{
    SO_NODE_HEADER(AirMirval);

public:
    static void initClass();
    AirMirval();

    double transmission(double distance) const;

    NAME_ICON_FUNCTIONS("Mirval", ":/AirMirval.png")
};


class AirMirvalFactory:
    public QObject, public AirFactoryT<AirMirval>
{
    Q_OBJECT
    Q_INTERFACES(AirFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.AirFactory")
};
