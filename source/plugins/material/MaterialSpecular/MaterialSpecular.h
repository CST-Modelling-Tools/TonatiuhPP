#pragma once

#include "kernel/material/MaterialAbstract.h"
#include "kernel/TonatiuhTypes.h"

class SoSensor;
class SoFieldSensor;


class MaterialSpecular: public MaterialAbstract
{
    SO_NODE_HEADER(MaterialSpecular);

public:
    enum Distribution {
        PILLBOX = 0,
        NORMAL = 1
    };

    static void initClass();
    MaterialSpecular();

    bool OutputRay(const Ray& rayIn, DifferentialGeometry* dg, RandomAbstract& rand, Ray* rayOut) const;

    SoSFDouble reflectivity;
    SoSFDouble slope;
    SoSFEnum distribution;

    NAME_ICON_FUNCTIONS("Specular", ":/MaterialSpecular.png")

protected:
    ~MaterialSpecular();

    double m_sigmaOpt;

    static void updateReflectivity(void* data, SoSensor*);

private:
    SoFieldSensor* m_reflectivitySensor;
};



#include "kernel/material/MaterialFactory.h"

class MaterialSpecularFactory:
    public QObject, public MaterialFactoryT<MaterialSpecular>
{
    Q_OBJECT
    Q_INTERFACES(MaterialFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.MaterialFactory")
};
