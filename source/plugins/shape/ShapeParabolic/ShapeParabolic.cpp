#include "ShapeParabolic.h"

#include "kernel/profiles/ProfileRT.h"
#include "kernel/profiles/ProfileRing.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/BoundingBox.h"
#include "libraries/geometry/Ray.h"

SO_NODE_SOURCE(ShapeParabolic)


void ShapeParabolic::initClass()
{
    SO_NODE_INIT_CLASS(ShapeParabolic, ShapeRT, "ShapeRT");
}

ShapeParabolic::ShapeParabolic()
{
    SO_NODE_CONSTRUCTOR(ShapeParabolic);

    SO_NODE_ADD_FIELD( focusX, (1.) );
    SO_NODE_ADD_FIELD( focusY, (1.) );
}

BoundingBox ShapeParabolic::getBox(ProfileRT* profile) const
{  
    BoundingBox box = profile->getBox();
    Vector3D v = box.absMax();
    box.pMax.z = (v.x*v.x/focusX.getValue() + v.y*v.y/focusY.getValue())/4.;
    return box;
}

bool ShapeParabolic::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const
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
            gcf::SevereError("ShapeParabolic::intersect");

        Vector3D dpdu(1., 0., pHit.x*gX/2.);
        Vector3D dpdv(0., 1., pHit.y*gY/2.);
        Vector3D normal(-pHit.x*gX, -pHit.y*gY, 2.);
        normal.normalize();
        bool isFront = dot(normal, rayD) <= 0.;

        *tHit = t;
        *dg = DifferentialGeometry(pHit, pHit.x, pHit.y, dpdu, dpdv, normal, this, isFront);
        return true;
    }
    return false;
}

void ShapeParabolic::updateShapeGL(TShapeKit* parent)
{
    ProfileRT* profile = (ProfileRT*) parent->profileRT.getValue();

    int rows, columns;
    double s;
    if (ProfileRing* pr = dynamic_cast<ProfileRing*>(profile))
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
        BoundingBox box = profile->getBox();
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

Vector3D ShapeParabolic::getPoint(double u, double v) const
{
    return Vector3D(
        u,
        v,
        (u*u/focusX.getValue() + v*v/focusY.getValue())/4.
    );
}

Vector3D ShapeParabolic::getNormal(double u, double v) const
{
    return Vector3D(
        -u/focusX.getValue(),
        -v/focusY.getValue(),
        2.
    ).normalized();
}
