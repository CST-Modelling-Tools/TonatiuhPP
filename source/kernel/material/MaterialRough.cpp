#include "MaterialRough.h"

#include "libraries/math/3D/Ray.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "kernel/random/Random.h"


SO_NODE_SOURCE(MaterialRough)

void MaterialRough::initClass()
{
     SO_NODE_INIT_CLASS(MaterialRough, MaterialRT, "MaterialRT");
}

MaterialRough::MaterialRough()
{
    SO_NODE_CONSTRUCTOR(MaterialRough);
    isBuiltIn = TRUE;

    SO_NODE_ADD_FIELD(diffuse, (1.) );
    SO_NODE_ADD_FIELD(specular, (0.) );
    SO_NODE_DEFINE_ENUM_VALUE(Distribution, Beckmann);
    SO_NODE_DEFINE_ENUM_VALUE(Distribution, Trowbridge);
    SO_NODE_SET_SF_ENUM_TYPE(distribution, Distribution);
    SO_NODE_ADD_FIELD(distribution, (Beckmann) );
    SO_NODE_ADD_FIELD(roughness, (0.) );

}


// http://www.pbr-book.org/3ed-2018/Light_Transport_I_Surface_Reflection/Sampling_Reflection_Functions.html
bool MaterialRough::OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& rand, Ray& rayOut) const
{
    if (rand.RandomDouble() <= diffuse.getValue())
    {
        rayOut.origin = dg.point;

        double phi = gcf::TwoPi*rand.RandomDouble();
        double sinTheta = sin(gcf::pi/2.)*sqrt(rand.RandomDouble());
        double cosTheta = sqrt(1. - sinTheta*sinTheta);
        vec3d dDiff(
            sinTheta*cos(phi),
            sinTheta*sin(phi),
            cosTheta
        );

        vec3d vx = dg.dpdu.normalized();
        vec3d vy = dg.dpdv.normalized();
        vec3d vz = dg.normal.normalized();
        dDiff = vx*dDiff.x + vy*dDiff.y + vz*dDiff.z;
        dDiff.normalize();

        if (dot(rayIn.direction(), vz) > 0.)
            dDiff = -dDiff;
        rayOut.setDirection(dDiff);
        return true;
    }
    else if (rand.RandomDouble() <= diffuse.getValue() + specular.getValue())
    {
        rayOut.origin = dg.point;

        vec3d normal;
        double alpha = roughness.getValue();
        if (alpha > 0.) {
            double phi = gcf::TwoPi*rand.RandomDouble();
            double tan2Theta = 0.;
            if (distribution.getValue() == Distribution::Beckmann)
            {
                tan2Theta = -gcf::pow2(alpha)*std::log(rand.RandomDouble());
            }
            else if (distribution.getValue() == Distribution::Trowbridge)
            {
                double u = rand.RandomDouble();
                tan2Theta = gcf::pow2(alpha)*u/(1. - u);
            }
            double cosTheta = 1./std::sqrt(1. + tan2Theta);
            double sinTheta = sqrt(1. - cosTheta*cosTheta);
            normal.x = sinTheta*cos(phi);
            normal.y = sinTheta*sin(phi);
            normal.z = cosTheta;

            vec3d vx = dg.dpdu.normalized();
            vec3d vy = dg.dpdv.normalized();
            vec3d vz = dg.normal.normalized(); // always normalized?
            normal = vx*normal.x + vy*normal.y + vz*normal.z;
            normal.normalize();
        } else
            normal = dg.normal;

        vec3d d = rayIn.direction().reflected(normal); // double sided
        bool qIn = dot(rayIn.direction(), dg.normal) < 0.;
        bool qOut = dot(d, dg.normal) < 0.;
        if (qIn == qOut) // fix for large roughness
            return false;
        rayOut.setDirection(d);
        return true;
    }

    return false;
}
