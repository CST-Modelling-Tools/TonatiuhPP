#include "ShapeCylinder.h"

#include "kernel/profiles/ProfileRT.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Ray.h"

SO_NODE_SOURCE(ShapeCylinder)


void ShapeCylinder::initClass()
{
    SO_NODE_INIT_CLASS(ShapeCylinder, ShapeRT, "ShapeRT");
}

ShapeCylinder::ShapeCylinder()
{
    SO_NODE_CONSTRUCTOR(ShapeCylinder);
}

vec3d ShapeCylinder::getPoint(double u, double v) const
{
    double phi = gcf::TwoPi*u;
    return vec3d(cos(phi), sin(phi), v);
}

vec3d ShapeCylinder::getNormal(double u, double v) const
{
    Q_UNUSED(v)
    double phi = gcf::TwoPi*u;
    return vec3d(cos(phi), sin(phi), 0.);
}

vec2d ShapeCylinder::getUV(const vec3d& p) const
{
    return vec2d(atan2(p.y, p.x)/gcf::TwoPi, p.z);
}

Box3D ShapeCylinder::getBox(ProfileRT* profile) const
{
    Box3D box = profile->getBox();
    double phiMin = gcf::TwoPi*box.pMin.x;
    double phiMax = gcf::TwoPi*box.pMax.x;
    double zMin = box.pMin.y;
    double zMax = box.pMax.y;

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

    return Box3D(
        vec3d(xMin, yMin, zMin),
        vec3d(xMax, yMax, zMax)
    );
}

bool ShapeCylinder::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const
{
    const vec3d& rayO = ray.origin;
    const vec3d& rayD = ray.direction();

    // |rxy|^2 = 1, r = r0 + t*d
    double A = rayD.x*rayD.x + rayD.y*rayD.y;
    double B = 2.*(rayD.x*rayO.x + rayD.y*rayO.y);
    double C = rayO.x*rayO.x + rayO.y*rayO.y - 1.;
    double ts[2];
    if (!gcf::solveQuadratic(A, B, C, &ts[0], &ts[1])) return false;

    for (int i = 0; i < 2; ++i)
    {
        double t = ts[i];
        if (t < ray.tMin + 1e-5 || t > ray.tMax) continue;

        vec3d pHit = ray.point(t);
        double phi = atan2(pHit.y, pHit.x);
        double u = phi/gcf::TwoPi;
        double v = pHit.z;
        if (!profile->isInside(u, v)) continue;

        if (tHit == 0 && dg == 0)
            return true;
        else if (tHit == 0 || dg == 0)
            gcf::SevereError("ShapeCylinder::intersect");

        *tHit = t;
        dg->point = pHit;
        dg->u = u;
        dg->v = v;
        dg->dpdu = vec3d(-pHit.y, pHit.x, 0.);
        dg->dpdv = vec3d(0., 0., 1.);
        dg->normal = vec3d(pHit.x, pHit.y, 0.);
        dg->shape = this;
        dg->isFront = dot(dg->normal, rayD) <= 0.;
        return true;
    }
    return false;
}

void ShapeCylinder::updateShapeGL(TShapeKit* parent)
{
    ProfileRT* aperture = (ProfileRT*) parent->profileRT.getValue();
    Box3D box = aperture->getBox();
    vec3d v = box.extent();

    double s = v.x;
    if (s > 1.) s = 1.;
    int rows = 1 + ceil(48*s);

    makeQuadMesh(parent, QSize(rows, 2));
}
