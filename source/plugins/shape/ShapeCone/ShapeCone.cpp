#include "ShapeCone.h"

#include "kernel/profiles/ProfileRT.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/math/Box3D.h"
#include "libraries/math/Ray.h"
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

Vector3D ShapeCone::getPoint(double u, double v) const
{
    double phi = gcf::TwoPi*u;
    double r = 1. + dr.getValue()*v;
    return Vector3D(r*cos(phi), r*sin(phi), v);
}

// |rxy|^2 = |1 + dr*z|^2
// [x, y, -(1 + dr*z)dr]
Vector3D ShapeCone::getNormal(double u, double v) const
{
    double phi = gcf::TwoPi*u;
    double drV = dr.getValue();
    double r = 1. + drV*v;
    r = r >= 0. ? 1. : -1;
    return Vector3D(r*cos(phi), r*sin(phi), -r*drV).normalized();
}

Vector2D ShapeCone::getUV(const Vector3D& p) const
{
    return Vector2D(atan2(p.y, p.x)/gcf::TwoPi, p.z);
}

Box3D ShapeCone::getBox(ProfileRT* aperture) const
{
    Box3D box = aperture->getBox();
    double zMin = box.pMin.y;
    double zMax = box.pMax.y;
    double drV = dr.getValue();
    double rMin = std::abs(1. + drV*zMin);
    double rMax = std::abs(1. + drV*zMax);
    if (rMin > rMax) std::swap(rMin, rMax);

    return Box3D(
        Vector3D(-rMax, -rMax, zMin),
        Vector3D(rMax, rMax, zMax)
    );
}

bool ShapeCone::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const
{
    const Vector3D& rayO = ray.origin;
    const Vector3D& rayD = ray.direction();
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

        Vector3D pHit = ray.point(t);
        double phi = atan2(pHit.y, pHit.x);
        double u = phi/gcf::TwoPi;
        double v = pHit.z;
        if (!aperture->isInside(u, v)) continue;

        if (tHit == 0 && dg == 0)
            return true;
        else if (tHit == 0 || dg == 0)
            gcf::SevereError("ShapeCone::intersect");

        double r = 1. + drV*v;
        if (gcf::eqz(r)) return false;

        *tHit = t;
        dg->point = pHit;
        dg->u = u;
        dg->v = v;
        dg->dpdu = Vector3D(-pHit.y, pHit.x, 0.);
        dg->dpdv = Vector3D(drV*pHit.x/r, drV*pHit.y/r, 1.);
        dg->normal = Vector3D(pHit.x, pHit.y, -drV*r).normalized();
        dg->shape = this;
        dg->isFront = dot(dg->normal, rayD) <= 0.;
        return true;
    }
    return false;
}

void ShapeCone::updateShapeGL(TShapeKit* parent)
{
    ProfileRT* aperture = (ProfileRT*) parent->profileRT.getValue();
    Box3D box = aperture->getBox();
    Vector3D v = box.extent();

    double s = v.x;
    if (s > 1.) s = 1.;
    int rows = 1 + ceil(48*s);

    makeQuadMesh(parent, QSize(rows, 2));
}
