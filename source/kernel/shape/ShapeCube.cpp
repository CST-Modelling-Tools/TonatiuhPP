#include "ShapeCube.h"

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>

#include "kernel/profiles/ProfileRT.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Ray.h"
#include "kernel/scene/MaterialGL.h"

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
    return Box3D::UnitCentered;
}

bool ShapeCube::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* aperture) const
{
    const vec3d& rayO = ray.origin;
    const vec3d& rayI = ray.invDirection();
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

        vec3d pHit = ray.point(t);
        Q_UNUSED(aperture)

        if (tHit == 0 && dg == 0)
            return true;
        else if (tHit == 0 || dg == 0)
            gcf::SevereError("ShapeCube::intersect");

        int s = pHit.abs().maxDimension();
        if (s == 0)
        {
            if (pHit[s] > 0.) {
                dg->dpdu = vec3d::UnitY;
                dg->dpdv = vec3d::UnitZ;
                dg->normal = vec3d::UnitX;
            } else {
                dg->dpdu = vec3d::UnitZ;
                dg->dpdv = vec3d::UnitY;
                dg->normal = -vec3d::UnitX;
            }
        }
        else if (s == 1)
        {
            if (pHit[s] > 0.) {
                dg->dpdu = vec3d::UnitZ;
                dg->dpdv = vec3d::UnitX;
                dg->normal = vec3d::UnitY;
            } else {
                dg->dpdu = vec3d::UnitX;
                dg->dpdv = vec3d::UnitZ;
                dg->normal = -vec3d::UnitY;
            }
        }
        else
        {
            if (pHit[s] > 0.) {
                dg->dpdu = vec3d::UnitX;
                dg->dpdv = vec3d::UnitY;
                dg->normal = vec3d::UnitZ;
            } else {
                dg->dpdu = vec3d::UnitY;
                dg->dpdv = vec3d::UnitX;
                dg->normal = -vec3d::UnitZ;
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
    MaterialGL* mGL = (MaterialGL*) parent->material.getValue();
    bool reverseNormals = mGL->reverseNormals.getValue();

    SoShapeKit* shapeKit = parent->m_shapeKit;
//    SoCube* cube = new SoCube;
//    cube->width = 1.;
//    cube->height = 1.;
//    cube->depth = 1.;
//    shapeKit->setPart("shape", cube);

    float sv[][3] = {
        {-0.5, -0.5, -0.5},
        {-0.5, -0.5, 0.5},
        {-0.5, 0.5, -0.5},
        {-0.5, 0.5, 0.5},
        {0.5, -0.5, -0.5},
        {0.5, -0.5, 0.5},
        {0.5, 0.5, -0.5},
        {0.5, 0.5, 0.5}
    };
    SoCoordinate3* sVertices = new SoCoordinate3;
    sVertices->point.setValues(0, 8, sv);
    shapeKit->setPart("coordinate3", sVertices);

    float q = reverseNormals ? -1 : 1;
    float nv[][3] = {
        {0, 0, -q},
        {0, 0, q},
        {0, -q, 0},
        {0, q, 0},
        {-q, 0, 0},
        {q, 0, 0}
    };

    SoNormal* sNormals = new SoNormal;
    sNormals->vector.setValues(0, 6, nv);
    shapeKit->setPart("normal", sNormals);

    SoNormalBinding* sb = new SoNormalBinding;
    sb->value = SoNormalBinding::PER_FACE;
    shapeKit->setPart("normalBinding", sb);

    int faces[] = {
        0, 2, 6, 4, -1,
        1, 3, 7, 5, -1,

        0, 1, 5, 4, -1,
        2, 3, 7, 6, -1,

        0, 1, 3, 2, -1,
        4, 5, 7, 6, -1
    };

    SoIndexedFaceSet* sMesh = new SoIndexedFaceSet;
    sMesh->coordIndex.setValues(0, 5*6, faces);
    shapeKit->setPart("shape", sMesh);
}
