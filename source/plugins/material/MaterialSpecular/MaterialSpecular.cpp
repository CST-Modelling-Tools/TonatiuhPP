#include "MaterialSpecular.h"

#include <QString>
#include <Inventor/sensors/SoFieldSensor.h>

#include "libraries/geometry/gc.h"
#include "libraries/geometry/Ray.h"
#include "libraries/geometry/Transform.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "kernel/random/RandomAbstract.h"
#include "kernel/TonatiuhFunctions.h"


SO_NODE_SOURCE(MaterialSpecular)

void MaterialSpecular::initClass()
{
    SO_NODE_INIT_CLASS(MaterialSpecular, MaterialAbstract, "Material");
}

MaterialSpecular::MaterialSpecular():
    m_sigmaOpt(0)
{
    SO_NODE_CONSTRUCTOR(MaterialSpecular);
    SO_NODE_ADD_FIELD(m_reflectivity, (0.) );
    SO_NODE_ADD_FIELD(m_sigmaSlope, (2.) );

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
    m_ambientColorSensor->attach(&m_ambientColor);
    m_diffuseColorSensor = new SoFieldSensor(updateDiffuseColor, this);
    m_diffuseColorSensor->attach(&m_diffuseColor);
    m_specularColorSensor = new SoFieldSensor(updateSpecularColor, this);
    m_specularColorSensor->attach(&m_specularColor);
    m_emissiveColorSensor = new SoFieldSensor(updateEmissiveColor, this);
    m_emissiveColorSensor->attach(&m_emissiveColor);
    m_shininessSensor = new SoFieldSensor(updateShininess, this);
    m_shininessSensor->attach(&m_shininess);
    m_transparencySensor = new SoFieldSensor(updateTransparency, this);
    m_transparencySensor->attach(&m_transparency);
}

MaterialSpecular::~MaterialSpecular()
{
    delete m_reflectivitySensor;
    delete m_ambientColorSensor;
    delete m_diffuseColorSensor;
    delete m_specularColorSensor;
    delete m_emissiveColorSensor;
    delete m_shininessSensor;
    delete m_transparencySensor;
}

void MaterialSpecular::updateReflectivity(void* data, SoSensor*)
{
    MaterialSpecular* material = static_cast< MaterialSpecular* >(data);
    if (material->m_reflectivity.getValue() < 0.0) material->m_reflectivity = 0.0;
    if (material->m_reflectivity.getValue() > 1.0) material->m_reflectivity = 1.0;
}

void MaterialSpecular::updateAmbientColor(void* data, SoSensor*)
{
    MaterialSpecular* material = static_cast< MaterialSpecular* >(data);
    material->ambientColor.setValue(material->m_ambientColor[0]);
}

void MaterialSpecular::updateDiffuseColor(void* data, SoSensor*)
{
    MaterialSpecular* material = static_cast< MaterialSpecular* >(data);
    material->diffuseColor.setValue(material->m_diffuseColor[0]);
}

void MaterialSpecular::updateSpecularColor(void* data, SoSensor*)
{
    MaterialSpecular* material = static_cast< MaterialSpecular* >(data);
    material->specularColor.setValue(material->m_specularColor[0]);
}

void MaterialSpecular::updateEmissiveColor(void* data, SoSensor*)
{
    MaterialSpecular* material = static_cast< MaterialSpecular* >(data);
    material->emissiveColor.setValue(material->m_emissiveColor[0]);
}

void MaterialSpecular::updateShininess(void* data, SoSensor*)
{
    MaterialSpecular* material = static_cast< MaterialSpecular* >(data);
    material->shininess.setValue(material->m_shininess[0]);
}

void MaterialSpecular::updateTransparency(void* data, SoSensor*)
{
    MaterialSpecular* material = static_cast< MaterialSpecular* >(data);
    material->transparency.setValue(material->m_transparency[0]);
}

bool MaterialSpecular::OutputRay(const Ray& incident, DifferentialGeometry* dg, RandomAbstract& rand, Ray* outputRay) const
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
        else if (m_distribution.getValue() == Distribution::NORMAL)
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

    double cosTheta = dot(normalVector, incident.direction() );
    outputRay->setDirection(Normalize(incident.direction() - 2. * normalVector * cosTheta) );
    return true;
}
