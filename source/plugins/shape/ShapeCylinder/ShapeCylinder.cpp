#include "ShapeCylinder.h"

#include "kernel/apertures/Aperture.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/BoundingBox.h"
#include "libraries/geometry/Ray.h"

SO_NODE_SOURCE(ShapeCylinder)


void ShapeCylinder::initClass()
{
    SO_NODE_INIT_CLASS(ShapeCylinder, ShapeRT, "ShapeRT");
}

ShapeCylinder::ShapeCylinder()
{
    SO_NODE_CONSTRUCTOR(ShapeCylinder);

    SO_NODE_ADD_FIELD( radius, (1.) );
}

BoundingBox ShapeCylinder::getBox(Aperture* aperture) const
{
    BoundingBox box = aperture->getBox();
    double phiMin = gcf::TwoPi*box.pMin.x;
    double phiMax = gcf::TwoPi*box.pMax.x;
    double zMin = box.pMin.y;
    double zMax = box.pMax.y;

    double r = radius.getValue();
    double xMin = r*cos(phiMin);
    double xMax = r*cos(phiMax);
    double yMin = r*sin(phiMin);
    double yMax = r*sin(phiMax);
    if (xMin > xMax) std::swap(xMin, xMax);
    if (yMin > yMax) std::swap(yMin, yMax);

    if (phiMin <= 0. && 0. <= phiMax)
        xMax = r;
    if (phiMin <= -gcf::pi || phiMax >= gcf::pi)
        xMin = -r;
    if (phiMin <= 0.5*gcf::pi && 0.5*gcf::pi <= phiMax)
        yMax = r;
    if (phiMin <= -0.5*gcf::pi && -0.5*gcf::pi <= phiMax)
        yMin = -r;

    return BoundingBox(
        Vector3D(xMin, yMin, zMin),
        Vector3D(xMax, yMax, zMax)
    );
}

bool ShapeCylinder::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, Aperture* aperture) const
{
    const Vector3D& rayO = ray.origin;
    const Vector3D& rayD = ray.direction();
    double r = radius.getValue();

    // |r0xy + dxy*t|^2 = R^2
    double A = rayD.x*rayD.x + rayD.y*rayD.y;
    double B = 2.*(rayD.x*rayO.x + rayD.y*rayO.y);
    double C = rayO.x*rayO.x + rayO.y*rayO.y - r*r;
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
            gcf::SevereError("ShapeCylinder::intersect");

        *tHit = t;
        dg->point = pHit;
        dg->u = u;
        dg->v = v;
        dg->dpdu = Vector3D(-pHit.y, pHit.x, 0.);
        dg->dpdv = Vector3D(0., 0., 1.);
        dg->normal = Vector3D(pHit.x, pHit.y, 0.)/r;
        dg->shape = this;
        dg->isFront = dot(dg->normal, rayD) <= 0.;
        return true;
    }
    return false;
}

void ShapeCylinder::updateShapeGL(TShapeKit* parent)
{
    Aperture* aperture = (Aperture*) parent->aperture.getValue();
    BoundingBox box = aperture->getBox();
    Vector3D v = box.extent();

    double s = v.x;
    if (s > 1.) s = 1.;
    int rows = 1 + ceil(48*s);

    makeQuadMesh(parent, QSize(rows, 2));
}

Vector3D ShapeCylinder::getPoint(double u, double v) const
{
    double r = radius.getValue();
    double phi = u*gcf::TwoPi;
    return Vector3D(r*cos(phi), r*sin(phi), v);
}

Vector3D ShapeCylinder::getNormal(double u, double v) const
{
    Q_UNUSED(v)
    double phi = u*gcf::TwoPi;
    return Vector3D(cos(phi), sin(phi), 0.);
}
