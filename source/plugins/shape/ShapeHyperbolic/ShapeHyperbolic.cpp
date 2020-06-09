#include "ShapeHyperbolic.h"

#include "kernel/profiles/ProfileRT.h"
#include "kernel/profiles/ProfileCircular.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/Box3D.h"
#include "libraries/geometry/Ray.h"
using gcf::pow2;

SO_NODE_SOURCE(ShapeHyperbolic)


void ShapeHyperbolic::initClass()
{
    SO_NODE_INIT_CLASS(ShapeHyperbolic, ShapeRT, "ShapeRT");
}

ShapeHyperbolic::ShapeHyperbolic()
{
    SO_NODE_CONSTRUCTOR(ShapeHyperbolic);

    SO_NODE_ADD_FIELD( aX, (1.) );
    SO_NODE_ADD_FIELD( aY, (1.) );
    SO_NODE_ADD_FIELD( aZ, (1.) );
}

Box3D ShapeHyperbolic::getBox(ProfileRT* profile) const
{  
    Box3D box = profile->getBox();
    Vector3D v = box.absMax();
    double rX = aX.getValue();
    double rY = aY.getValue();
    double rZ = aZ.getValue();
    double s = 1. + pow2(v.x/rX) + pow2(v.y/rY);
    s = sqrt(s) - 1.;
    box.pMax.z = s*rZ;
    return box;
}

bool ShapeHyperbolic::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const
{
    double rZ = aZ.getValue();
    Vector3D g(1./aX.getValue(), 1./aY.getValue(), 1./rZ);
    Vector3D rayO = (ray.origin + Vector3D(0., 0., rZ))*g;
    Vector3D rayD = ray.direction()*g;

    double A = pow2(rayD.x) + pow2(rayD.y) - pow2(rayD.z);
    double B = 2.*(rayD.x*rayO.x + rayD.y*rayO.y - rayD.z*rayO.z);
    double C = pow2(rayO.x) + pow2(rayO.y) - pow2(rayO.z) + 1.;
    double ts[2];
    if (!gcf::solveQuadratic(A, B, C, &ts[0], &ts[1])) return false;

    for (int i = 0; i < 2; ++i)
    {
        double t = ts[i];
        if (t < ray.tMin + 1e-5 || t > ray.tMax) continue;

        Vector3D pHit = ray.point(t);
        if (pHit.z < 0.) continue; // discard lower branch
        if (!profile->isInside(pHit.x, pHit.y)) continue;

        if (tHit == 0 && dg == 0)
            return true;
        else if (tHit == 0 || dg == 0)
            gcf::SevereError("ShapeHyperbolic::intersect");

        *tHit = t;
        dg->point = pHit;
        dg->u = pHit.x;
        dg->v = pHit.y;
        double s = 1. + pHit.z/rZ;
        dg->dpdu = Vector3D(1., 0., pHit.x/s*g.x*g.x*rZ);
        dg->dpdv = Vector3D(0., 1., pHit.y/s*g.y*g.y*rZ);
        dg->normal = Vector3D(-dg->dpdu.z, -dg->dpdv.z, 1.).normalized();
        dg->shape = this;
        dg->isFront = dot(dg->normal, ray.direction()) <= 0.;
        return true;
    }
    return false;
}

void ShapeHyperbolic::updateShapeGL(TShapeKit* parent)
{
    ProfileRT* profile = (ProfileRT*) parent->profileRT.getValue();

    int rows, columns;
    double s;
    if (ProfileCircular* pr = dynamic_cast<ProfileCircular*>(profile))
    {
        s = (pr->phiMax.getValue() - pr->phiMin.getValue())/gcf::TwoPi;
        if (s > 1.) s = 1.;
        rows = 1 + ceil(48*s);

        double r = std::min(aX.getValue(), aY.getValue());
        s = (pr->rMax.getValue() - pr->rMin.getValue())/(gcf::TwoPi*r);
        if (s > 1.) s = 1.;
        columns = 1 + ceil(48*s);
    }
    else
    {
        Box3D box = profile->getBox();
        Vector3D v = box.extent();

        // 48 divs for 2 pi
        s = v.x/(gcf::TwoPi*aX.getValue());
        if (s > 1.) s = 1.;
        rows = 1 + ceil(48*s);

        s = v.y/(gcf::TwoPi*aY.getValue());
        if (s > 1.) s = 1.;
        columns = 1 + ceil(48*s);
    }

    makeQuadMesh(parent, QSize(rows, columns));
}

Vector3D ShapeHyperbolic::getPoint(double u, double v) const
{
    double rX = aX.getValue();
    double rY = aY.getValue();
    double rZ = aZ.getValue();
    double s = 1. + pow2(u/rX) + pow2(v/rY);
    s = sqrt(s) - 1.;
    return Vector3D(u, v, s*rZ);
}

Vector3D ShapeHyperbolic::getNormal(double u, double v) const
{
    double rX = aX.getValue();
    double rY = aY.getValue();
    double rZ = aZ.getValue();
    double s = 1. + pow2(u/rX) + pow2(v/rY);
    return Vector3D(-u/(rX*rX), -v/(rY*rY), sqrt(s)/rZ).normalized();
}
