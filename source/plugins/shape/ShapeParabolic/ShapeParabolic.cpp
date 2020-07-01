#include "ShapeParabolic.h"

#include "kernel/profiles/ProfileRT.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Ray.h"
using gcf::pow2;

SO_NODE_SOURCE(ShapeParabolic)


void ShapeParabolic::initClass()
{
    SO_NODE_INIT_CLASS(ShapeParabolic, ShapeRT, "ShapeRT");
}

ShapeParabolic::ShapeParabolic()
{
    SO_NODE_CONSTRUCTOR(ShapeParabolic);

    SO_NODE_ADD_FIELD( fX, (1.) );
    SO_NODE_ADD_FIELD( fY, (1.) );
}

vec3d ShapeParabolic::getPoint(double u, double v) const
{
    return vec3d(
        u,
        v,
        (u*u/fX.getValue() + v*v/fY.getValue())/4.
    );
}

vec3d ShapeParabolic::getNormal(double u, double v) const
{
    return vec3d(
        -u/fX.getValue(),
        -v/fY.getValue(),
        2.
    ).normalized();
}

Box3D ShapeParabolic::getBox(ProfileRT* profile) const
{  
    Box3D box = profile->getBox();
    vec3d v = box.absMax();
    double zX = v.x*v.x/(4.*fX.getValue());
    double zY = v.y*v.y/(4.*fY.getValue());
    if (zX >= 0.) {
        if (zY >= 0.)
            box.pMax.z = zX + zY;
        else {
            box.pMax.z = zX;
            box.pMin.z = zY;
        }
    } else {
        if (zY >= 0.) {
            box.pMax.z = zY;
            box.pMin.z = zX;
        } else
            box.pMin.z = zX + zY;
    }
    return box;
}

// x^2*gx + y^2*gy = 4z
// r = r0 + d*t
bool ShapeParabolic::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const
{
    const vec3d& rayO = ray.origin;
    const vec3d& rayD = ray.direction();
    double gX = 1./fX.getValue();
    double gY = 1./fY.getValue();

    double A = pow2(rayD.x)*gX + pow2(rayD.y)*gY;
    double B = 2.*(rayD.x*rayO.x*gX + rayD.y*rayO.y*gY) - 4.*rayD.z;
    double C = pow2(rayO.x)*gX + pow2(rayO.y)*gY - 4.*rayO.z;
    double ts[2];
    if (!gcf::solveQuadratic(A, B, C, &ts[0], &ts[1])) return false;

    for (int i = 0; i < 2; ++i)
    {
        double t = ts[i];
        if (t < ray.tMin + 1e-5 || t > ray.tMax) continue;

        vec3d pHit = ray.point(t);
        if (!profile->isInside(pHit.x, pHit.y)) continue;

        if (tHit == 0 && dg == 0)
            return true;
        else if (tHit == 0 || dg == 0)
            gcf::SevereError("ShapeParabolic::intersect");

        *tHit = t;
        dg->point = pHit;
        dg->u = pHit.x;
        dg->v = pHit.y;
        dg->dpdu = vec3d(1., 0., pHit.x*gX/2.);
        dg->dpdv = vec3d(0., 1., pHit.y*gY/2.);
        dg->normal = vec3d(-dg->dpdu.z, -dg->dpdv.z, 1.).normalized();
        dg->shape = this;
        dg->isFront = dot(dg->normal, rayD) <= 0.;
        return true;
    }
    return false;
}

void ShapeParabolic::updateShapeGL(TShapeKit* parent)
{
    ProfileRT* profile = (ProfileRT*) parent->profileRT.getValue();
    Box3D box = profile->getBox();
    vec3d q = box.absMin();
    vec3d s = box.extent();

    double cx = 2.*std::abs(fX.getValue());
    double cy = 2.*std::abs(fY.getValue());
    double sx = 0.1*cx*(1. + pow2(q.x/cx));
    double sy = 0.1*cy*(1. + pow2(q.y/cy));
    int rows = 1 + ceil(s.x/sx);
    int columns = 1 + ceil(s.y/sy);

    makeQuadMesh(parent, QSize(rows, columns));
}
