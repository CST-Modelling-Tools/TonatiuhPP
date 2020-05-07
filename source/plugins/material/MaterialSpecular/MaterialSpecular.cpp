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
    SO_NODE_ADD_FIELD(reflectivity, (0.9) );
    SO_NODE_ADD_FIELD(sigmaSlope, (2.) );

    //SO_NODE_DEFINE_ENUM_VALUE(Distribution, PILLBOX);
    SO_NODE_DEFINE_ENUM_VALUE(Distribution, NORMAL);
    SO_NODE_SET_SF_ENUM_TYPE(distribution, Distribution);
    SO_NODE_ADD_FIELD(distribution, (NORMAL) );

    SO_NODE_ADD_FIELD(ambientColor, (0.2f, 0.2f, 0.2f) );
    SO_NODE_ADD_FIELD(diffuseColor, (0.8f, 0.8f, 0.8f) );
    SO_NODE_ADD_FIELD(specularColor, (0.0, 0.0, 0.0) );
    SO_NODE_ADD_FIELD(emissiveColor, (0.0, 0.0, 0.0) );
    SO_NODE_ADD_FIELD(shininess, (0.2f) );
    SO_NODE_ADD_FIELD(transparency, (0.0f) );

    m_reflectivitySensor = new SoFieldSensor(updateReflectivity, this);
    m_reflectivitySensor->attach(&reflectivity);

//    m_ambientColorSensor = new SoFieldSensor(updateAmbientColor, this);
//    m_ambientColorSensor->attach(&m_ambientColor);
//    m_diffuseColorSensor = new SoFieldSensor(updateDiffuseColor, this);
//    m_diffuseColorSensor->attach(&m_diffuseColor);
//    m_specularColorSensor = new SoFieldSensor(updateSpecularColor, this);
//    m_specularColorSensor->attach(&m_specularColor);
//    m_emissiveColorSensor = new SoFieldSensor(updateEmissiveColor, this);
//    m_emissiveColorSensor->attach(&m_emissiveColor);
//    m_shininessSensor = new SoFieldSensor(updateShininess, this);
//    m_shininessSensor->attach(&m_shininess);
//    m_transparencySensor = new SoFieldSensor(updateTransparency, this);
//    m_transparencySensor->attach(&m_transparency);
}

MaterialSpecular::~MaterialSpecular()
{
    delete m_reflectivitySensor;
//    delete m_ambientColorSensor;
//    delete m_diffuseColorSensor;
//    delete m_specularColorSensor;
//    delete m_emissiveColorSensor;
//    delete m_shininessSensor;
//    delete m_transparencySensor;
}

bool MaterialSpecular::OutputRay(const Ray& rayIn, DifferentialGeometry* dg, RandomAbstract& rand, Ray* rayOut) const
{
    double randomNumber = rand.RandomDouble();
    if (randomNumber >= reflectivity.getValue() ) return false;

    // Compute reflected ray (local coordinates)
    rayOut->origin = dg->point;

    Vector3D normal;
    double slope = sigmaSlope.getValue() / 1000.; // from mrad to rad
    if (slope > 0.) {
        Vector3D errorNormal;
        if (distribution.getValue() == 0)
        { // pillbox
            double phi = gc::TwoPi*rand.RandomDouble();
            double theta = slope*rand.RandomDouble();

            errorNormal.x = sin(theta)*sin(phi);
            errorNormal.y = sin(theta)*cos(phi);
            errorNormal.z = cos(theta);
        }
        else if (distribution.getValue() == Distribution::NORMAL)
        { // normal
            errorNormal.x = slope*tgf::AlternateBoxMuller(rand);
            errorNormal.y = slope*tgf::AlternateBoxMuller(rand);
            errorNormal.z = 1.;
        }
        Vector3D vx = dg->dpdu.normalized();
        Vector3D vy = dg->dpdv.normalized();
        Vector3D vz = dg->normal;
        normal = (vx*errorNormal.x + vy*errorNormal.y + vz*errorNormal.z).normalized();
    } else
        normal = dg->normal;

    Vector3D d = rayIn.direction() - 2.*normal*dot(normal, rayIn.direction());
    rayOut->setDirection(d.normalized());
    return true;
}

void MaterialSpecular::updateReflectivity(void* data, SoSensor*)
{
    MaterialSpecular* material = static_cast<MaterialSpecular*>(data);
    if (material->reflectivity.getValue() < 0.)
        material->reflectivity = 0.;
    if (material->reflectivity.getValue() > 1.)
        material->reflectivity = 1.;
}

/*
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
}*/
