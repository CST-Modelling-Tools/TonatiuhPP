#include "ShapeSphere.h"

#include "kernel/profiles/ProfileBox.h"
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

ProfileRT* ShapeSphere::getDefaultProfile() const
{
    ProfileBox* pr = new ProfileBox;
    pr->uSize = 360.;
    pr->vSize = 180.;
    return pr;
}

vec3d ShapeSphere::getPoint(double u, double v) const
{
    double phi = gcf::degree*u;
    double alpha = gcf::degree*v;
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
    return vec2d(phi, alpha)/gcf::degree;
}


Box3D ShapeSphere::getBox(ProfileRT* profile) const
{
    Box2D box = profile->getBox();
    double phiMin = gcf::degree*box.min().x;
    double phiMax = gcf::degree*box.max().x;
    double alphaMin = gcf::degree*gcf::clamp(box.min().y, -90., 90.);
    double alphaMax = gcf::degree*gcf::clamp(box.max().y, -90., 90.);

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

// https://en.wikipedia.org/wiki/Second_fundamental_form
double ShapeSphere::getStepHint(double u, double v) const
{
    Q_UNUSED(u)
    Q_UNUSED(v)

//    r = n = {cosu cosv, sinu cosv, sinv};
//    ru = {-sinu cosv, cosu cosv, 0};
//    rv = {-cosu sinv, -sinu sinv, cosv};
//    ruu = {-cosu cosv, -sinu cosv, 0};
//    ruv = {sinu sinv, -cosu sinv, 0};
//    rvv = -r;

//    L = ruu.n = -cosv^2;
//    M = ruv.n = 0;
//    N = rvv.n = -1;

//    double t1, t2;
//    gcf::solveQuadratic(1., -(L + N), L*N - M*M, &t1, &t2);
//    double radius = 1./std::max(std::abs(t1), std::abs(t2));
//    double radius = std::min(std::abs(1./L), std::abs(1./N));

//    double radius = 1.;
//    return 2*gcf::pi*radius/48/gcf::degree;

    return 360./48;
}

void ShapeSphere::updateShapeGL(TShapeKit* parent)
{
    ProfileRT* profile = (ProfileRT*) parent->profileRT.getValue();
    vec2d v = profile->getBox().size()/360.;

    int rows = 1 + ceil(48*v.x);
    int columns = 1 + ceil(48*v.y);
    makeQuadMesh(parent, QSize(rows, columns));
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
        vec2d uv = getUV(pHit);
        if (!profile->isInside(uv.x, uv.y)) continue;

        if (tHit == 0 && dg == 0)
            return true;
        else if (tHit == 0 || dg == 0)
            gcf::SevereError("ShapeSphere::intersect");

        *tHit = t;
        dg->point = pHit;
        dg->uv = uv;
        dg->dpdu = vec3d(-pHit.y, pHit.x, 0.);
        double phi = uv.x*gcf::degree;
        double alpha = uv.y*gcf::degree;
        dg->dpdv = vec3d(-cos(phi)*pHit.z, -sin(phi)*pHit.z, cos(alpha));
        dg->normal = pHit;
        dg->shape = this;
        dg->isFront = dot(dg->normal, rayD) <= 0.;
        return true;
    }
    return false;
}
