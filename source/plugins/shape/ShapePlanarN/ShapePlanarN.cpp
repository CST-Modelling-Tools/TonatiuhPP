#include "ShapePlanarN.h"

#include "kernel/profiles/ProfileRT.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Ray.h"

SO_NODE_SOURCE(ShapePlanarN)


void ShapePlanarN::initClass()
{
    SO_NODE_INIT_CLASS(ShapePlanarN, ShapeRT, "ShapeRT");
}

// https://en.wikipedia.org/wiki/Normal_mapping
ShapePlanarN::ShapePlanarN()
{
    SO_NODE_CONSTRUCTOR(ShapePlanarN);

    SO_NODE_ADD_FIELD( xLimits, (-0.5f, 0.5f) );
    SO_NODE_ADD_FIELD( yLimits, (-0.5f, 0.5f) );
    SO_NODE_ADD_FIELD( dims, (2, 2) );

    float ns[][3] = {
        {0., 0., 1.},
        {0., 0., 1.},
        {0., 0., 1.},
        {0., 0., 1.}
    };
    normals.setValues(0, 4, ns);
    normals.setContainer(this);
    fieldData->addField(this, "normals", &normals);

    SO_NODE_ADD_FIELD( fX, (1.) );
    SO_NODE_ADD_FIELD( fY, (1.) );
}

// interpolation normals
vec3d ShapePlanarN::getNormal(double u, double v) const
{
    return vec3d(
        -u/fX.getValue(),
        -v/fY.getValue(),
        2.
    ).normalized();
}

bool ShapePlanarN::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const
{
    // r0_z + d_z*t = 0
    double t = -ray.origin.z*ray.invDirection().z;

    if (t < ray.tMin + 1e-5 || t > ray.tMax) return false;

    vec3d pHit = ray.point(t);
    if (!profile->isInside(pHit.x, pHit.y)) return false;

    if (tHit == 0 && dg == 0)
        return true;
    else if (tHit == 0 || dg == 0)
        gcf::SevereError("ShapePlanar::intersect");

//    vec3d vN(0.1, 0.2, 1.);
//    vN.normalize();
//    vec3d vU(vN.z, 0., -vN.x);
//    vU.normalize();
//    vec3d vV = cross(vN, vU);

    *tHit = t;
    dg->point = pHit;
    dg->u = pHit.x;
    dg->v = pHit.y;
    dg->dpdu = vec3d(1., 0., 0.);
    dg->dpdv = vec3d(0., 1., 0.);
    dg->normal = vec3d(0., 0., 1.); // interpolate
    dg->shape = this;
    dg->isFront = dot(dg->normal, ray.direction()) <= 0.;
    return true;
}

void ShapePlanarN::updateShapeGL(TShapeKit* parent)
{
    makeQuadMesh(parent, QSize(2, 2));
}
