#include <QString>

#include <Inventor/sensors/SoFieldSensor.h>

#include "MaterialStandardSpecular.h"
#include "libraries/geometry/gc.h"
#include "libraries/geometry/Ray.h"
#include "libraries/geometry/Transform.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "kernel/random/RandomAbstract.h"
#include "kernel/tgf.h"


SO_NODE_SOURCE(MaterialStandardSpecular)

void MaterialStandardSpecular::initClass()
{
    SO_NODE_INIT_CLASS(MaterialStandardSpecular, MaterialAbstract, "Material");
}

MaterialStandardSpecular::MaterialStandardSpecular():
    m_sigmaOpt(0)
{
    SO_NODE_CONSTRUCTOR(MaterialStandardSpecular);
    SO_NODE_ADD_FIELD(m_reflectivity, (0.0) );
    SO_NODE_ADD_FIELD(m_sigmaSlope, (2.0) );

    //SO_NODE_DEFINE_ENUM_VALUE(Distribution, PILLBOX);
    SO_NODE_DEFINE_ENUM_VALUE(Distribution, NORMAL);
    SO_NODE_SET_SF_ENUM_TYPE(m_distribution, Distribution);
    SO_NODE_ADD_FIELD(m_distribution, (NORMAL) );

    SO_NODE_ADD_FIELD(m_ambientColor, (0.2f, 0.2f, 0.2f) );
    SO_NODE_ADD_FIELD(m_diffuseColor, (0.8f, 0.8f, 0.8f) );
    SO_NODE_ADD_FIELD(m_specularColor, (0.0, 0.0, 0.0) );
    SO_NODE_ADD_FIELD(m_emissiveColor, (0.0, 0.0, 0.0) );
    SO_NODE_ADD_FIELD(m_shininess, (0.2f) );
    SO_NODE_ADD_FIELD(m_transparency, (0.0f) );

    m_reflectivitySensor = new SoFieldSensor(updateReflectivity, this);
    m_reflectivitySensor->setPriority(1);
    m_reflectivitySensor->attach(&m_reflectivity);

    m_ambientColorSensor = new SoFieldSensor(updateAmbientColor, this);
    m_ambientColorSensor->setPriority(1);
    m_ambientColorSensor->attach(&m_ambientColor);
    m_diffuseColorSensor = new SoFieldSensor(updateDiffuseColor, this);
    m_diffuseColorSensor->setPriority(1);
    m_diffuseColorSensor->attach(&m_diffuseColor);
    m_specularColorSensor = new SoFieldSensor(updateSpecularColor, this);
    m_specularColorSensor->setPriority(1);
    m_specularColorSensor->attach(&m_specularColor);
    m_emissiveColorSensor = new SoFieldSensor(updateEmissiveColor, this);
    m_emissiveColorSensor->setPriority(1);
    m_emissiveColorSensor->attach(&m_emissiveColor);
    m_shininessSensor = new SoFieldSensor(updateShininess, this);
    m_shininessSensor->setPriority(1);
    m_shininessSensor->attach(&m_shininess);
    m_transparencySensor = new SoFieldSensor(updateTransparency, this);
    m_transparencySensor->setPriority(1);
    m_transparencySensor->attach(&m_transparency);
}

MaterialStandardSpecular::~MaterialStandardSpecular()
{
    delete m_reflectivitySensor;
    delete m_ambientColorSensor;
    delete m_diffuseColorSensor;
    delete m_specularColorSensor;
    delete m_emissiveColorSensor;
    delete m_shininessSensor;
    delete m_transparencySensor;
}

QString MaterialStandardSpecular::getIcon()
{
    return ":/MaterialStandardSpecular.png";
}

void MaterialStandardSpecular::updateReflectivity(void* data, SoSensor*)
{
    MaterialStandardSpecular* material = static_cast< MaterialStandardSpecular* >(data);
    if (material->m_reflectivity.getValue() < 0.0) material->m_reflectivity = 0.0;
    if (material->m_reflectivity.getValue() > 1.0) material->m_reflectivity = 1.0;
}

void MaterialStandardSpecular::updateAmbientColor(void* data, SoSensor*)
{
    MaterialStandardSpecular* material = static_cast< MaterialStandardSpecular* >(data);
    material->ambientColor.setValue(material->m_ambientColor[0]);
}

void MaterialStandardSpecular::updateDiffuseColor(void* data, SoSensor*)
{
    MaterialStandardSpecular* material = static_cast< MaterialStandardSpecular* >(data);
    material->diffuseColor.setValue(material->m_diffuseColor[0]);
}

void MaterialStandardSpecular::updateSpecularColor(void* data, SoSensor*)
{
    MaterialStandardSpecular* material = static_cast< MaterialStandardSpecular* >(data);
    material->specularColor.setValue(material->m_specularColor[0]);
}

void MaterialStandardSpecular::updateEmissiveColor(void* data, SoSensor*)
{
    MaterialStandardSpecular* material = static_cast< MaterialStandardSpecular* >(data);
    material->emissiveColor.setValue(material->m_emissiveColor[0]);
}

void MaterialStandardSpecular::updateShininess(void* data, SoSensor*)
{
    MaterialStandardSpecular* material = static_cast< MaterialStandardSpecular* >(data);
    material->shininess.setValue(material->m_shininess[0]);
}

void MaterialStandardSpecular::updateTransparency(void* data, SoSensor*)
{
    MaterialStandardSpecular* material = static_cast< MaterialStandardSpecular* >(data);
    material->transparency.setValue(material->m_transparency[0]);
}

bool MaterialStandardSpecular::OutputRay(const Ray& incident, DifferentialGeometry* dg, RandomAbstract& rand, Ray* outputRay) const
{
    double randomNumber = rand.RandomDouble();
    if (randomNumber >= m_reflectivity.getValue() ) return false;

    //Compute reflected ray (local coordinates )
    outputRay->origin = dg->point;

    Vector3D normalVector;
    double sigmaSlope = m_sigmaSlope.getValue() / 1000.; // from mrad to rad
    if (sigmaSlope > 0.) {
        Vector3D errorNormal;
        if (m_distribution.getValue() == 0)
        { // pillbox
            double phi = gc::TwoPi * rand.RandomDouble();
            double theta = sigmaSlope * rand.RandomDouble();

            errorNormal.x = sin(theta) * sin(phi);
            errorNormal.y = cos(theta);
            errorNormal.z = sin(theta) * cos(phi);
        }
        else if (m_distribution.getValue() == 1)
        { // normal
            errorNormal.x = sigmaSlope * tgf::AlternateBoxMuller(rand);
            errorNormal.y = 1.;
            errorNormal.z = sigmaSlope * tgf::AlternateBoxMuller(rand);
        }
        Vector3D r = dg->normal;
        Vector3D s = Normalize(dg->dpdu);
        Vector3D t = Normalize(dg->dpdv);
        normalVector = Normalize(s*errorNormal.x + r*errorNormal.y + t*errorNormal.z);
    } else
        normalVector = dg->normal;

    double cosTheta = DotProduct(normalVector, incident.direction() );
    outputRay->setDirection(Normalize(incident.direction() - 2. * normalVector * cosTheta) );
    return true;
}
