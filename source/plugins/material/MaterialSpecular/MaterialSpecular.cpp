#include "MaterialSpecular.h"

#include <Inventor/sensors/SoNodeSensor.h>

#include "libraries/geometry/gcf.h"
#include "libraries/geometry/Ray.h"
#include "libraries/geometry/Transform.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "kernel/random/Random.h"
#include "kernel/TonatiuhFunctions.h"


SO_NODE_SOURCE(MaterialSpecular)

void MaterialSpecular::initClass()
{
    SO_NODE_INIT_CLASS(MaterialSpecular, MaterialRT, "MaterialRT");
}

MaterialSpecular::MaterialSpecular()
{
    SO_NODE_CONSTRUCTOR(MaterialSpecular);

    SO_NODE_ADD_FIELD(reflectivity, (0.9) );

    //SO_NODE_DEFINE_ENUM_VALUE(Distribution, pillbox);
    SO_NODE_DEFINE_ENUM_VALUE(Distribution, Gaussian);
    SO_NODE_SET_SF_ENUM_TYPE(distribution, Distribution);
    SO_NODE_ADD_FIELD(distribution, (Gaussian) );

    SO_NODE_ADD_FIELD(slope, (0.002) ); // in radians

    m_sensor = new SoNodeSensor(onSensor, this);
    m_sensor->attach(this);
}

MaterialSpecular::~MaterialSpecular()
{
    delete m_sensor;
}

bool MaterialSpecular::OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& rand, Ray& rayOut) const
{
    // reflectivity
    if (rand.RandomDouble() >= reflectivity.getValue()) return false;

    rayOut.origin = dg.point;

    Vector3D normal;
    double sigma = slope.getValue();
    if (sigma > 0.) {
        if (distribution.getValue() == Distribution::pillbox)
        {
            double phi = gcf::TwoPi*rand.RandomDouble();
            double theta = sigma*rand.RandomDouble();
            normal.x = sin(theta)*cos(phi);
            normal.y = sin(theta)*sin(phi);
            normal.z = cos(theta);
        }
        else if (distribution.getValue() == Distribution::Gaussian)
        {
            // https://en.wikipedia.org/wiki/Marsaglia_polar_method
            double u, v, s;
            do {
                u = 2.*rand.RandomDouble() - 1.;
                v = 2.*rand.RandomDouble() - 1.;
                s = u*u + v*v;
            } while (s > 1. || s == 0.);
            s = sigma*sqrt(-2.*log(s)/s);

            normal.x = s*u;
            normal.y = s*v;
            normal.z = 1.;
        }
        Vector3D vx = dg.dpdu.normalized();
        Vector3D vy = dg.dpdv.normalized();
//        Vector3D vz = dg.normal;
        Vector3D vz = dg.normal.normalized();
        normal = (vx*normal.x + vy*normal.y + vz*normal.z).normalized();
    } else
//        normal = dg.normal;
        normal = dg.normal.normalized();

    Vector3D d = rayIn.direction() - 2.*normal*dot(normal, rayIn.direction());
    rayOut.setDirection(d); // double sided
    return true;
}

void MaterialSpecular::onSensor(void* data, SoSensor*)
{
    MaterialSpecular* material = (MaterialSpecular*) data;
    if (material->reflectivity.getValue() < 0.)
        material->reflectivity = 0.;
    if (material->reflectivity.getValue() > 1.)
        material->reflectivity = 1.;
}
