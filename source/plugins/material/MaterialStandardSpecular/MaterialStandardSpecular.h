#pragma once

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFString.h>

#include "kernel/material/MaterialAbstract.h"
#include "kernel/raytracing/trt.h"

class SoSensor;
class SoFieldSensor;


class MaterialStandardSpecular: public MaterialAbstract
{
    SO_NODE_HEADER(MaterialStandardSpecular);

public:
    enum Distribution {
        //PILLBOX = 0,
        NORMAL = 1,
    };

    MaterialStandardSpecular();
	static void initClass();

    QString getIcon();
    bool OutputRay(const Ray& incident, DifferentialGeometry* dg, RandomAbstract& rand, Ray* outputRay) const;

	trt::TONATIUH_REAL m_reflectivity;
	trt::TONATIUH_REAL m_sigmaSlope;
	SoSFEnum m_distribution;

    SoMFColor m_ambientColor;
    SoMFColor m_diffuseColor;
    SoMFColor m_specularColor;
    SoMFColor m_emissiveColor;
	SoMFFloat m_shininess;
    SoMFFloat m_transparency;

    static const char* getClassName() {return "StandardSpecular";}
    static const char* getClassIcon() {return ":/MaterialStandardSpecular.png";}

protected:
    virtual ~MaterialStandardSpecular();

    double m_sigmaOpt;

    static void updateReflectivity(void* data, SoSensor*);
    static void updateAmbientColor(void* data, SoSensor*);
    static void updateDiffuseColor(void* data, SoSensor*);
    static void updateSpecularColor(void* data, SoSensor*);
    static void updateEmissiveColor(void* data, SoSensor*);
    static void updateShininess(void* data, SoSensor*);
    static void updateTransparency(void* data, SoSensor*);

private:
    SoFieldSensor* m_reflectivitySensor;
    SoFieldSensor* m_ambientColorSensor;
    SoFieldSensor* m_diffuseColorSensor;
    SoFieldSensor* m_specularColorSensor;
    SoFieldSensor* m_emissiveColorSensor;
    SoFieldSensor* m_shininessSensor;
    SoFieldSensor* m_transparencySensor;
};



#include "kernel/material/MaterialFactory.h"

class MaterialStandardSpecularFactory:
    public QObject, public MaterialFactoryT<MaterialStandardSpecular>
{
    Q_OBJECT
    Q_INTERFACES(MaterialFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.MaterialFactory")
};
