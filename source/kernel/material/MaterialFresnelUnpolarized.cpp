#include "MaterialFresnelUnpolarized.h"

#include "libraries/math/3D/Ray.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "kernel/random/Random.h"


SO_NODE_SOURCE(MaterialFresnelUnpolarized)

void MaterialFresnelUnpolarized::initClass()
{
     SO_NODE_INIT_CLASS(MaterialFresnelUnpolarized, MaterialRT, "MaterialRT");
}

MaterialFresnelUnpolarized::MaterialFresnelUnpolarized()
{
    SO_NODE_CONSTRUCTOR(MaterialFresnelUnpolarized);
//    isBuiltIn = TRUE;
    SO_NODE_ADD_FIELD( nFront, (1.) );// air
    SO_NODE_ADD_FIELD( nBack, (1.5) );// glass

    SO_NODE_DEFINE_ENUM_VALUE(Distribution, pillbox);
    SO_NODE_DEFINE_ENUM_VALUE(Distribution, Gaussian);
    SO_NODE_SET_SF_ENUM_TYPE(distribution, Distribution);
    SO_NODE_ADD_FIELD(distribution, (Gaussian) );

    SO_NODE_ADD_FIELD(slope, (0.002) ); // in radians
}

bool MaterialFresnelUnpolarized::OutputRay(const Ray& rayIn, const DifferentialGeometry& dg, Random& rand, Ray& rayOut) const
{
    Q_UNUSED(rand)

    // surface roughness
    vec3d normal;
    double sigma = slope.getValue();
    if (sigma > 0.) {
        if (distribution.getValue() == Distribution::pillbox)
        {
            double phi = gcf::TwoPi*rand.RandomDouble();
            double sinTheta = sin(sigma)*sqrt(rand.RandomDouble());
            double cosTheta = sqrt(1. - sinTheta*sinTheta);
            normal.x = sinTheta*cos(phi);
            normal.y = sinTheta*sin(phi);
            normal.z = cosTheta;
        }
        else //if (distribution.getValue() == Distribution::Gaussian)
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
        vec3d vx = dg.dpdu.normalized();
        vec3d vy = dg.dpdv.normalized();
        vec3d vz = dg.normal.normalized();
        normal = vx*normal.x + vy*normal.y + vz*normal.z;
        normal.normalize();
    } else
        normal = dg.normal.normalized();

    // select sides according to incident ray
    const vec3d& dI = rayIn.direction();
    double dIn = dot(dI, normal);
    double nI = nFront.getValue();
    double nT = nBack.getValue();
    if (dIn > 0.) std::swap(nI, nT);
    dIn = std::abs(dIn);

    // reflection law
    double dIt = std::sqrt(1. - dIn*dIn);
    double temp = 1. - gcf::pow2(nI/nT*dIt);
    double dTn = 0.;
    double R = 1.;
    if (temp > 0.) { // no total reflection
        dTn = std::sqrt(temp);
        // averaged Fresnel coefficients
        double Rs = (nI*dIn - nT*dTn)/(nI*dIn + nT*dTn);
        double Rp = (nT*dIn - nI*dTn)/(nT*dIn + nI*dTn);
        R = (Rs*Rs + Rp*Rp)/2.;
    }

    vec3d dO;
    if (rand.RandomDouble() <= R) { // reflection
        dO = dI.reflected(normal);
    } else { // transmission
        dO = (nI/nT)*dI.projected(normal) - dTn*normal;
    }
    rayOut.origin = dg.point;
    rayOut.setDirection(dO);
    return true;
}
