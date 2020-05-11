#include "MaterialSpecular.h"

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
    SO_NODE_INIT_CLASS(MaterialSpecular, MaterialAbstract, "MaterialAbstract");
}

MaterialSpecular::MaterialSpecular():
    m_sigmaOpt(0)
{
    SO_NODE_CONSTRUCTOR(MaterialSpecular);

    SO_NODE_ADD_FIELD(reflectivity, (0.9) );
    SO_NODE_ADD_FIELD(slope, (2.) );

    //SO_NODE_DEFINE_ENUM_VALUE(Distribution, PILLBOX);
    SO_NODE_DEFINE_ENUM_VALUE(Distribution, NORMAL);
    SO_NODE_SET_SF_ENUM_TYPE(distribution, Distribution);
    SO_NODE_ADD_FIELD(distribution, (NORMAL) );

    SO_NODE_ADD_FIELD( ambientColor, (0.2f, 0.2f, 0.2f) );
    SO_NODE_ADD_FIELD( diffuseColor, (0.8f, 0.8f, 0.8f) );
    SO_NODE_ADD_FIELD( specularColor, (0.f, 0.f, 0.f) );
    SO_NODE_ADD_FIELD( emissiveColor, (0.f, 0.f, 0.f) );
    SO_NODE_ADD_FIELD( shininess, (0.2f) );
    SO_NODE_ADD_FIELD( transparency, (0.f) );

    m_reflectivitySensor = new SoFieldSensor(updateReflectivity, this);
    m_reflectivitySensor->attach(&reflectivity);
}

MaterialSpecular::~MaterialSpecular()
{
    delete m_reflectivitySensor;
}

bool MaterialSpecular::OutputRay(const Ray& rayIn, DifferentialGeometry* dg, RandomAbstract& rand, Ray* rayOut) const
{
    double randomNumber = rand.RandomDouble();
    if (randomNumber >= reflectivity.getValue()) return false;

    // Compute reflected ray (local coordinates)
    rayOut->origin = dg->point;

    Vector3D normal;
    double sigma = slope.getValue()/1000.; // from mrad to rad
    if (sigma > 0.) {
        if (distribution.getValue() == Distribution::PILLBOX)
        {
            double phi = gc::TwoPi*rand.RandomDouble();
            double theta = sigma*rand.RandomDouble();
            normal.x = sin(theta)*sin(phi);
            normal.y = sin(theta)*cos(phi);
            normal.z = cos(theta);
        }
        else if (distribution.getValue() == Distribution::NORMAL)
        {
            normal.x = sigma*tgf::AlternateBoxMuller(rand);
            normal.y = sigma*tgf::AlternateBoxMuller(rand);
            normal.z = 1.;
        }
        Vector3D vx = dg->dpdu.normalized();
        Vector3D vy = dg->dpdv.normalized();
        Vector3D vz = dg->normal;
        normal = (vx*normal.x + vy*normal.y + vz*normal.z).normalized();
    } else
        normal = dg->normal;

    Vector3D d = rayIn.direction() - 2.*normal*dot(normal, rayIn.direction());
    rayOut->setDirection(d.normalized()); // double sided
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
