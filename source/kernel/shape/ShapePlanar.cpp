#include "ShapePlanar.h"

#include "kernel/profiles/ProfileRT.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Ray.h"

SO_NODE_SOURCE(ShapePlanar)


void ShapePlanar::initClass()
{
    SO_NODE_INIT_CLASS(ShapePlanar, ShapeRT, "ShapeRT");
}

ShapePlanar::ShapePlanar()
{
    SO_NODE_CONSTRUCTOR(ShapePlanar);
}

bool ShapePlanar::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const
{
    // r0_z + d_z*t = 0
    if (ray.origin.z == 0 && ray.direction().z == 0) return false;
    double t = -ray.origin.z*ray.invDirection().z;

    if (t < ray.tMin + 1e-5 || t > ray.tMax) return false;

    vec3d pHit = ray.point(t);
    if (!profile->isInside(pHit.x, pHit.y)) return false;

    if (tHit == 0 && dg == 0)
        return true;
    else if (tHit == 0 || dg == 0)
        gcf::SevereError("ShapePlanar::intersect");

    *tHit = t;
    dg->point = pHit;
    dg->u = pHit.x;
    dg->v = pHit.y;
    dg->dpdu = vec3d(1., 0., 0.);
    dg->dpdv = vec3d(0., 1., 0.);
    dg->normal = vec3d(0., 0., 1.);
    dg->shape = this;
    dg->isFront = dot(dg->normal, ray.direction()) <= 0.;
    return true;
}

void ShapePlanar::updateShapeGL(TShapeKit* parent)
{
    makeQuadMesh(parent, QSize(2, 2));
}
