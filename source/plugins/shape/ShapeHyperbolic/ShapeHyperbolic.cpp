#include "ShapeHyperbolic.h"

#include "kernel/profiles/ProfileRT.h"
#include "kernel/profiles/ProfileCircular.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/Box3D.h"
#include "libraries/geometry/Ray.h"

SO_NODE_SOURCE(ShapeHyperbolic)


void ShapeHyperbolic::initClass()
{
    SO_NODE_INIT_CLASS(ShapeHyperbolic, ShapeRT, "ShapeRT");
}

ShapeHyperbolic::ShapeHyperbolic()
{
    SO_NODE_CONSTRUCTOR(ShapeHyperbolic);

    SO_NODE_ADD_FIELD( focusX, (1.) );
    SO_NODE_ADD_FIELD( focusY, (1.) );
}

Box3D ShapeHyperbolic::getBox(ProfileRT* profile) const
{  
    Box3D box = profile->getBox();
    Vector3D v = box.absMax();
    box.pMax.z = (v.x*v.x/focusX.getValue() + v.y*v.y/focusY.getValue())/4.;
    return box;
}

bool ShapeHyperbolic::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const
{
    const Vector3D& rayO = ray.origin;
    const Vector3D& rayD = ray.direction();
    double gX = 1./focusX.getValue();
    double gY = 1./focusY.getValue();

    // (x0 + t*d_x)^2*gX + (y0 + t*d_y)^2*gY = 4*(z0 + t*d_z)
    double A = rayD.x*rayD.x*gX + rayD.y*rayD.y*gY;
    double B = 2.*(rayD.x*rayO.x*gX + rayD.y*rayO.y*gY) - 4.*rayD.z;
    double C = rayO.x*rayO.x*gX + rayO.y*rayO.y*gY - 4.*rayO.z;
    double ts[2];
    if (!gcf::solveQuadratic(A, B, C, &ts[0], &ts[1])) return false;

    for (int i = 0; i < 2; ++i)
    {
        double t = ts[i];
        if (t < ray.tMin + 1e-5 || t > ray.tMax) continue;

        Vector3D pHit = ray.point(t);
        if (!profile->isInside(pHit.x, pHit.y)) continue;

        if (tHit == 0 && dg == 0)
            return true;
        else if (tHit == 0 || dg == 0)
            gcf::SevereError("ShapeHyperbolic::intersect");

        *tHit = t;
        dg->point = pHit;
        dg->u = pHit.x;
        dg->v = pHit.y;
        dg->dpdu = Vector3D(1., 0., pHit.x*gX/2.);
        dg->dpdv = Vector3D(0., 1., pHit.y*gY/2.);
        dg->normal = Vector3D(-pHit.x*gX, -pHit.y*gY, 2.).normalized();
        dg->shape = this;
        dg->isFront = dot(dg->normal, rayD) <= 0.;
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

        double f = std::min(focusX.getValue(), focusY.getValue());
        s = (pr->rMax.getValue() - pr->rMin.getValue())/(gcf::TwoPi*2.*f);
        if (s > 1.) s = 1.;
        columns = 1 + ceil(48*s);
    }
    else
    {
        Box3D box = profile->getBox();
        Vector3D v = box.extent();

        // radius = 2*focus, 48 divs for 2 pi
        s = v.x/(gcf::TwoPi*2.*focusX.getValue());
        if (s > 1.) s = 1.;
        rows = 1 + ceil(48*s);

        s = v.y/(gcf::TwoPi*2.*focusY.getValue());
        if (s > 1.) s = 1.;
        columns = 1 + ceil(48*s);
    }

    makeQuadMesh(parent, QSize(rows, columns));
}

Vector3D ShapeHyperbolic::getPoint(double u, double v) const
{
    return Vector3D(
        u,
        v,
        (u*u/focusX.getValue() + v*v/focusY.getValue())/4.
    );
}

Vector3D ShapeHyperbolic::getNormal(double u, double v) const
{
    return Vector3D(
        -u/focusX.getValue(),
        -v/focusY.getValue(),
        2.
    ).normalized();
}
