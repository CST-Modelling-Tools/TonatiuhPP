#pragma once

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFString.h>

#include "kernel/material/MaterialAbstract.h"
#include "kernel/TonatiuhTypes.h"

class SoSensor;
class SoFieldSensor;


class MaterialSpecular: public MaterialAbstract
{
    SO_NODE_HEADER(MaterialSpecular);

public:
    enum Distribution {
        //PILLBOX = 0,
        NORMAL = 1,
    };

    static void initClass();
    MaterialSpecular();

    bool OutputRay(const Ray& rayIn, DifferentialGeometry* dg, RandomAbstract& rand, Ray* rayOut) const;

    SoSFDouble reflectivity;
    SoSFDouble sigmaSlope;
    SoSFEnum distribution;

//    SoMFColor m_ambientColor;
//    SoMFColor m_diffuseColor;
//    SoMFColor m_specularColor;
//    SoMFColor m_emissiveColor;
//	SoMFFloat m_shininess;
//    SoMFFloat m_transparency;

    NAME_ICON_FUNCTIONS("Specular", ":/MaterialSpecular.png")

protected:
    ~MaterialSpecular();

    double m_sigmaOpt;

    static void updateReflectivity(void* data, SoSensor*);
//    static void updateAmbientColor(void* data, SoSensor*);
//    static void updateDiffuseColor(void* data, SoSensor*);
//    static void updateSpecularColor(void* data, SoSensor*);
//    static void updateEmissiveColor(void* data, SoSensor*);
//    static void updateShininess(void* data, SoSensor*);
//    static void updateTransparency(void* data, SoSensor*);

private:
    SoFieldSensor* m_reflectivitySensor;
//    SoFieldSensor* m_ambientColorSensor;
//    SoFieldSensor* m_diffuseColorSensor;
//    SoFieldSensor* m_specularColorSensor;
//    SoFieldSensor* m_emissiveColorSensor;
//    SoFieldSensor* m_shininessSensor;
//    SoFieldSensor* m_transparencySensor;
};



#include "kernel/material/MaterialFactory.h"

class MaterialSpecularFactory:
    public QObject, public MaterialFactoryT<MaterialSpecular>
{
    Q_OBJECT
    Q_INTERFACES(MaterialFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.MaterialFactory")
};
