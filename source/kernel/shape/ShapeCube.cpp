#include "ShapeCube.h"

#include <Inventor/nodes/SoCube.h>

#include "kernel/profiles/ProfileRT.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/Box3D.h"
#include "libraries/geometry/Ray.h"

SO_NODE_SOURCE(ShapeCube)


void ShapeCube::initClass()
{
    SO_NODE_INIT_CLASS(ShapeCube, ShapeRT, "ShapeRT");
}

ShapeCube::ShapeCube()
{
    SO_NODE_CONSTRUCTOR(ShapeCube);
}

Box3D ShapeCube::getBox(ProfileRT* aperture) const
{
    Q_UNUSED(aperture)
    return Box3D::UnitCube;
}

bool ShapeCube::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const
{
    const Vector3D& rayO = ray.origin;
    const Vector3D& rayI = ray.invDirection();
    double tMin, tMax, tyMin, tyMax, tzMin, tzMax;
    double h = 0.5;

    if (rayI.x >= 0.)
    {
        tMin = (-h - rayO.x)*rayI.x;
        tMax = (h - rayO.x)*rayI.x;
    }
    else
    {
        tMin = (h - rayO.x)*rayI.x;
        tMax = (-h - rayO.x)*rayI.x;
    }

    if (rayI.y >= 0.)
    {
        tyMin = (-h - rayO.y)*rayI.y;
        tyMax = (h - rayO.y)*rayI.y;
    }
    else
    {
        tyMin = (h - rayO.y)*rayI.y;
        tyMax = (-h - rayO.y)*rayI.y;
    }
    if (tyMin > tMax || tyMax < tMin) return false;
    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    if (rayI.z >= 0.)
    {
        tzMin = (-h - rayO.z)*rayI.z;
        tzMax = (h - rayO.z)*rayI.z;
    }
    else
    {
        tzMin = (h - rayO.z)*rayI.z;
        tzMax = (-h - rayO.z)*rayI.z;
    }
    if (tzMin > tMax || tzMax < tMin) return false;
    if (tzMin > tMin) tMin = tzMin;
    if (tzMax < tMax) tMax = tzMax;

    double ts[2];
    ts[0] = tMin;
    ts[1] = tMax;
    for (int i = 0; i < 2; ++i)
    {
        double t = ts[i];
        if (t < ray.tMin + 1e-5 || t > ray.tMax) continue;

        Vector3D pHit = ray.point(t);
        Q_UNUSED(aperture)

        if (tHit == 0 && dg == 0)
            return true;
        else if (tHit == 0 || dg == 0)
            gcf::SevereError("ShapeCube::intersect");

        int s = pHit.abs().maxDimension();
        if (s == 0)
        {
            if (pHit[s] > 0.) {
                dg->dpdu = Vector3D::UnitY;
                dg->dpdv = Vector3D::UnitZ;
                dg->normal = Vector3D::UnitX;
            } else {
                dg->dpdu = Vector3D::UnitZ;
                dg->dpdv = Vector3D::UnitY;
                dg->normal = -Vector3D::UnitX;
            }
        }
        else if (s == 1)
        {
            if (pHit[s] > 0.) {
                dg->dpdu = Vector3D::UnitZ;
                dg->dpdv = Vector3D::UnitX;
                dg->normal = Vector3D::UnitY;
            } else {
                dg->dpdu = Vector3D::UnitX;
                dg->dpdv = Vector3D::UnitZ;
                dg->normal = -Vector3D::UnitY;
            }
        }
        else
        {
            if (pHit[s] > 0.) {
                dg->dpdu = Vector3D::UnitX;
                dg->dpdv = Vector3D::UnitY;
                dg->normal = Vector3D::UnitZ;
            } else {
                dg->dpdu = Vector3D::UnitY;
                dg->dpdv = Vector3D::UnitX;
                dg->normal = -Vector3D::UnitZ;
            }
        }

        *tHit = tMin;
        dg->point = pHit;
        dg->u = 0.; //?
        dg->v = 0.; //?
        dg->shape = this;
        dg->isFront = dot(dg->normal,  ray.direction()) <= 0.;
        return true;
    }
    return false;
}

void ShapeCube::updateShapeGL(TShapeKit* parent)
{
    SoCube* cube = new SoCube;
    cube->width = 1.;
    cube->height = 1.;
    cube->depth = 1.;
    parent->setPart("shape", cube);
}
