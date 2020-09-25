#include "ShapeSphere.h"

#include "kernel/profiles/ProfileRT.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Ray.h"

SO_NODE_SOURCE(ShapeSphere)


void ShapeSphere::initClass()
{
    SO_NODE_INIT_CLASS(ShapeSphere, ShapeRT, "ShapeRT");
}

ShapeSphere::ShapeSphere()
{
	SO_NODE_CONSTRUCTOR(ShapeSphere);
    isBuiltIn = TRUE;
}

vec3d ShapeSphere::getPoint(double u, double v) const
{
    double phi = gcf::TwoPi*u;
    double alpha = gcf::pi*v;
    return vec3d(
        cos(phi)*cos(alpha),
        sin(phi)*cos(alpha),
        sin(alpha)
    );
}

vec3d ShapeSphere::getNormal(double u, double v) const
{
    return getPoint(u, v);
}

vec2d ShapeSphere::getUV(const vec3d& p) const
{
    double phi = atan2(p.y, p.x);
    double alpha = asin(gcf::clamp(p.z, -1., 1.));
    return vec2d(phi/gcf::TwoPi, alpha/gcf::pi);
}

Box3D ShapeSphere::getBox(ProfileRT* profile) const
{
    Box2D box = profile->getBox();
    double phiMin = gcf::TwoPi*box.min().x;
    double phiMax = gcf::TwoPi*box.max().x;
    double alphaMin = gcf::pi*gcf::clamp(box.min().y, -0.5, 0.5);
    double alphaMax = gcf::pi*gcf::clamp(box.max().y, -0.5, 0.5);

    double rMin = cos(alphaMin);
    double rMax = cos(alphaMax);
    if (rMin > rMax) std::swap(rMin, rMax);
    if (alphaMin <= 0. && 0. <= alphaMax)
        rMax = 1.;

    double xMin = cos(phiMin);
    double xMax = cos(phiMax);
    double yMin = sin(phiMin);
    double yMax = sin(phiMax);
    if (xMin > xMax) std::swap(xMin, xMax);
    if (yMin > yMax) std::swap(yMin, yMax);

    if (phiMin <= 0. && 0. <= phiMax)
        xMax = 1.;
    if (phiMin <= -gcf::pi || phiMax >= gcf::pi)
        xMin = -1.;
    if (phiMin <= 0.5*gcf::pi && 0.5*gcf::pi <= phiMax)
        yMax = 1.;
    if (phiMin <= -0.5*gcf::pi && -0.5*gcf::pi <= phiMax)
        yMin = -1.;

    xMax *= xMax > 0. ? rMax : rMin;
    xMin *= xMin > 0. ? rMin : rMax;

    yMax *= yMax > 0. ? rMax : rMin;
    yMin *= yMin > 0. ? rMin : rMax;

    double zMin = sin(alphaMin);
    double zMax = sin(alphaMax);

    return Box3D(
        vec3d(xMin, yMin, zMin),
        vec3d(xMax, yMax, zMax)
    );
}

bool ShapeSphere::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const
{
    const vec3d& rayO = ray.origin;
    const vec3d& rayD = ray.direction();

    // |r|^2 = 1, r = r0 + t*d
    double A = rayD.norm2();
    double B = 2.*dot(rayD, rayO);
    double C = rayO.norm2() - 1.;
    double ts[2];
    if (!gcf::solveQuadratic(A, B, C, &ts[0], &ts[1])) return false;

    for (int i = 0; i < 2; ++i)
    {
        double t = ts[i];
        if (t < ray.tMin + 1e-5 || t > ray.tMax) continue;

        vec3d pHit = ray.point(t);
        double phi = atan2(pHit.y, pHit.x);
        double alpha = asin(gcf::clamp(pHit.z, -1., 1.));
        double u = phi/gcf::TwoPi;
        double v = alpha/gcf::pi;
        if (!profile->isInside(u, v)) continue;

        if (tHit == 0 && dg == 0)
            return true;
        else if (tHit == 0 || dg == 0)
            gcf::SevereError("ShapeSphere::intersect");

        *tHit = t;
        dg->point = pHit;
        dg->u = u;
        dg->v = v;
        dg->dpdu = vec3d(-pHit.y, pHit.x, 0.);
        dg->dpdv = vec3d(-cos(phi)*pHit.z, -sin(phi)*pHit.z, cos(alpha));
        dg->normal = pHit;
        dg->shape = this;
        dg->isFront = dot(dg->normal, rayD) <= 0.;
        return true;
    }
    return false;
}

void ShapeSphere::updateShapeGL(TShapeKit* parent)
{
    ProfileRT* aperture = (ProfileRT*) parent->profileRT.getValue();
    Box2D box = aperture->getBox();
    vec2d v = box.size();

    double s = v.x;
    if (s > 1.) s = 1.;
    int rows = 1 + ceil(48*s);

    s = v.y;
    if (s > 1.) s = 1.;
    int columns = 1 + ceil(24*s);

    makeQuadMesh(parent, QSize(rows, columns));
}
