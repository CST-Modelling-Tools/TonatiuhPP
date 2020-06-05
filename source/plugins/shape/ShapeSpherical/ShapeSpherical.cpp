#include "ShapeSpherical.h"

#include "kernel/profiles/ProfileRT.h"
#include "kernel/profiles/ProfileCircular.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/Box3D.h"
#include "libraries/geometry/Ray.h"
using gcf::pow2;

SO_NODE_SOURCE(ShapeSpherical)


void ShapeSpherical::initClass()
{
    SO_NODE_INIT_CLASS(ShapeSpherical, ShapeRT, "ShapeRT");
}

ShapeSpherical::ShapeSpherical()
{
    SO_NODE_CONSTRUCTOR(ShapeSpherical);

    SO_NODE_ADD_FIELD( radiusXY, (1.) );
    SO_NODE_ADD_FIELD( radiusZ, (1.) );
}

Box3D ShapeSpherical::getBox(ProfileRT* profile) const
{  
    Box3D box = profile->getBox();
    Vector3D v = box.absMax();
    double r = radiusXY.getValue();
    double rZ = radiusZ.getValue();
    double s = 1. - pow2(v.x/r) - pow2(v.y/r);
    s = 1. - sqrt(s);
    box.pMax.z = rZ*s;
    return box;
}

// (x/rX)^2 + (y/rY)^2 + (z/rZ - 1)^2 = 1
// r = r0 + d*t
bool ShapeSpherical::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const
{
    double r = radiusXY.getValue();
    double rz = radiusZ.getValue();
    double gxy = 1./r;
    double gz = 1./rz;
    Vector3D rayO = (ray.origin - Vector3D(0., 0., rz))*Vector3D(gxy, gxy, gz);
    Vector3D rayD = ray.direction()*Vector3D(gxy, gxy, gz);

    double A = rayD.norm2();
    double B = 2.*dot(rayD, rayO);
    double C = rayO.norm2() - 1.;
    double ts[2];
    if (!gcf::solveQuadratic(A, B, C, &ts[0], &ts[1])) return false;

    for (int i = 0; i < 2; ++i)
    {
        double t = ts[i];
        if (t < ray.tMin + 1e-5 || t > ray.tMax) continue;

        Vector3D pHit = ray.point(t);
        if (pHit.z > rz) continue;
        if (!profile->isInside(pHit.x, pHit.y)) continue;

        if (tHit == 0 && dg == 0)
            return true;
        else if (tHit == 0 || dg == 0)
            gcf::SevereError("ShapeSpherical::intersect");

        *tHit = t;
        dg->point = pHit;
        dg->u = pHit.x;
        dg->v = pHit.y;
        double zp = 1. - pHit.z/rz;
        dg->dpdu = Vector3D(1., 0., pHit.x/zp);
        dg->dpdv = Vector3D(0., 1., pHit.y/zp);
        dg->normal = Vector3D(-pHit.x, -pHit.y, zp).normalized();
        dg->shape = this;
        dg->isFront = dot(dg->normal, ray.direction()) <= 0.;
        return true;
    }
    return false;
}

// z/rZ = [(x/rX)^2 + (y/rY)^2]/[1 + sqrt[1 - (x/rX)^2 - (y/rY)^2]]
// z = x^2/(4f)
// f = rX^2/(2rZ)

// use curvature or subdivision
// https://en.wikipedia.org/wiki/Differential_geometry_of_surfaces
void ShapeSpherical::updateShapeGL(TShapeKit* parent)
{
    ProfileRT* profile = (ProfileRT*) parent->profileRT.getValue();

    int rows, columns;
    double s;
    if (ProfileCircular* pr = dynamic_cast<ProfileCircular*>(profile))
    {
        s = (pr->phiMax.getValue() - pr->phiMin.getValue())/gcf::TwoPi;
        if (s > 1.) s = 1.;
        rows = 1 + ceil(48*s);

        s = (pr->rMax.getValue() - pr->rMin.getValue())/(gcf::TwoPi*radiusXY.getValue());
        if (s > 1.) s = 1.;
        columns = 1 + ceil(48*s);
    }
    else
    {
        Box3D box = profile->getBox();
        Vector3D v = box.extent();

        // 48 divs for 2 pi
        s = v.x/(gcf::TwoPi*radiusXY.getValue());
        if (s > 1.) s = 1.;
        rows = 1 + ceil(48*s);

        s = v.y/(gcf::TwoPi*radiusXY.getValue());
        if (s > 1.) s = 1.;
        columns = 1 + ceil(48*s);
    }

    makeQuadMesh(parent, QSize(rows, columns));
}

// (x/rX)^2 + (y/rY)^2 + (z/rZ - 1)^2 = 1
// z/rZ = 1 - sqrt[1 - (x/rX)^2 - (y/rY)^2]
Vector3D ShapeSpherical::getPoint(double u, double v) const
{
    double r = radiusXY.getValue();
    double rZ = radiusZ.getValue();
    double s = 1. - pow2(u/r) - pow2(v/r);
    s = 1. - sqrt(s);
    return Vector3D(u, v, rZ*s);
}

// [x/rx^2, y/rY^2, (z/rZ - 1)/rZ]
Vector3D ShapeSpherical::getNormal(double u, double v) const
{
    double r = radiusXY.getValue();
    double rZ = radiusZ.getValue();
    double s = pow2(r) - pow2(u) - pow2(v);
    return Vector3D(-u, -v, (r/rZ)*sqrt(s)).normalized();
}
