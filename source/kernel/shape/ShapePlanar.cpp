#include "ShapePlanar.h"

#include "kernel/profiles/ProfileRT.h"
#include "kernel/profiles/ProfileRing.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/BoundingBox.h"
#include "libraries/geometry/Ray.h"

SO_NODE_SOURCE(ShapePlanar)


void ShapePlanar::initClass()
{
    SO_NODE_INIT_CLASS(ShapePlanar, ShapeRT, "ShapeRT");
}

ShapePlanar::ShapePlanar()
{
    SO_NODE_CONSTRUCTOR(ShapePlanar);
}

BoundingBox ShapePlanar::getBox(ProfileRT* aperture) const
{
    BoundingBox box = aperture->getBox();
    double zMax = 0.01*box.extent().max();
    box.pMin.z = -zMax;
    box.pMax.z = zMax;
    return box;
}

bool ShapePlanar::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const
{
    // r0_z + d_z*t = 0
    if (ray.origin.z == 0 && ray.direction().z == 0) return false;
    double t = -ray.origin.z*ray.invDirection().z;

    if (t < ray.tMin + 1e-5 || t > ray.tMax) return false;

    Vector3D pHit = ray.point(t);
    if (!aperture->isInside(pHit.x, pHit.y)) return false;

    if (tHit == 0 && dg == 0)
        return true;
    else if (tHit == 0 || dg == 0)
        gcf::SevereError("ShapePlanar::intersect");

    *tHit = t;
    dg->point = pHit;
    dg->u = pHit.x;
    dg->v = pHit.y;
    dg->dpdu = Vector3D(1., 0., 0.);
    dg->dpdv = Vector3D(0., 1., 0.);
    dg->normal = Vector3D(0., 0., 1.);
    dg->shape = this;
    dg->isFront = dot(dg->normal, ray.direction()) <= 0.;
    return true;
}

void ShapePlanar::updateShapeGL(TShapeKit* parent)
{
    ProfileRT* profile = (ProfileRT*) parent->profileRT.getValue();

    int rows;
    if (ProfileRing* pr = dynamic_cast<ProfileRing*>(profile))
    {
        double s = (pr->phiMax.getValue() - pr->phiMin.getValue())/gcf::TwoPi;
        if (s > 1.) s = 1.;
        rows = 1 + ceil(48*s);
    }
    else
    {
        rows = 2;
    }

    makeQuadMesh(parent, QSize(rows, 2));
}

Vector3D ShapePlanar::getPoint(double u, double v) const
{
    return Vector3D(u, v, 0.);
}

Vector3D ShapePlanar::getNormal(double u, double v) const
{
    Q_UNUSED(u)
    Q_UNUSED(v)
    return Vector3D(0., 0., 1.);
}
