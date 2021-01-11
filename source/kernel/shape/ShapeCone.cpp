#include "ShapeCone.h"

#include "kernel/profiles/ProfileBox.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Ray.h"
using gcf::pow2;

SO_NODE_SOURCE(ShapeCone)


void ShapeCone::initClass()
{
    SO_NODE_INIT_CLASS(ShapeCone, ShapeRT, "ShapeRT");
}

ShapeCone::ShapeCone()
{
    SO_NODE_CONSTRUCTOR(ShapeCone);

    SO_NODE_ADD_FIELD( dr, (-1.) );
}

vec3d ShapeCone::getPoint(double u, double v) const
{
    double r = 1. + dr.getValue()*v;
    return vec3d(r*cos(u), r*sin(u), v);
}

// |rxy|^2 = |1 + dr*z|^2
// [x, y, -(1 + dr*z)dr]
vec3d ShapeCone::getNormal(double u, double v) const
{
    double drV = dr.getValue();
    double r = 1. + drV*v;
    r = r >= 0. ? 1. : -1;
    return vec3d(r*cos(u), r*sin(u), -r*drV).normalized();
}

ProfileRT* ShapeCone::getDefaultProfile() const
{
    ProfileBox* pr = new ProfileBox;
    pr->uSize.set("360d");
    return pr;
}

vec2d ShapeCone::getUV(const vec3d& p) const
{
    return vec2d(atan2(p.y, p.x), p.z);
}

Box3D ShapeCone::getBox(ProfileRT* aperture) const
{
    Box2D box = aperture->getBox();
    double zMin = box.min().y;
    double zMax = box.max().y;
    double drV = dr.getValue();
    double rMin = std::abs(1. + drV*zMin);
    double rMax = std::abs(1. + drV*zMax);
    if (rMin > rMax) std::swap(rMin, rMax);

    return Box3D(
        vec3d(-rMax, -rMax, zMin),
        vec3d(rMax, rMax, zMax)
    );
}

bool ShapeCone::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const
{
    const vec3d& rayO = ray.origin;
    const vec3d& rayD = ray.direction();
    double drV = dr.getValue();

    // |rxy|^2 = |1 + dr*z|^2, r = r0 + t*d
    double rz = 1. + drV*rayO.z;
    double dz = drV*rayD.z;
    double A = pow2(rayD.x) + pow2(rayD.y) - pow2(dz);
    double B = 2.*(rayD.x*rayO.x + rayD.y*rayO.y - dz*rz);
    double C = pow2(rayO.x) + pow2(rayO.y) - pow2(rz);
    double ts[2];
    if (!gcf::solveQuadratic(A, B, C, &ts[0], &ts[1])) return false;

    for (int i = 0; i < 2; ++i)
    {
        double t = ts[i];
        if (t < ray.tMin + 1e-5 || t > ray.tMax) continue;

        vec3d pHit = ray.point(t);
        vec2d uv = getUV(pHit);
        if (!aperture->isInside(uv.x, uv.y)) continue;

        if (tHit == 0 && dg == 0)
            return true;
        else if (tHit == 0 || dg == 0)
            gcf::SevereError("ShapeCone::intersect");

        double r = 1. + drV*uv.y;
        if (gcf::eqz(r)) return false;

        *tHit = t;
        dg->point = pHit;
        dg->uv = uv;
        dg->dpdu = vec3d(-pHit.y, pHit.x, 0.);
        dg->dpdv = vec3d(drV*pHit.x/r, drV*pHit.y/r, 1.);
        dg->normal = vec3d(pHit.x, pHit.y, -drV*r).normalized();
        dg->shape = this;
        dg->isFront = dot(dg->normal, rayD) <= 0.;
        return true;
    }
    return false;
}

void ShapeCone::updateShapeGL(TShapeKit* parent)
{
    ProfileRT* profile = (ProfileRT*) parent->profileRT.getValue();
    Box2D box = profile->getBox();
    vec2d v = box.size();

    double s = v.x/(2*gcf::pi);
    if (s > 1.) s = 1.;
    int rows = 1 + ceil(48*s);

    makeQuadMesh(parent, QSize(rows, 2));
}
