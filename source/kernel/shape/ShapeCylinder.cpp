#include "ShapeCylinder.h"

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoQuadMesh.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>

#include "kernel/profiles/ProfileBox.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Ray.h"

SO_NODE_SOURCE(ShapeCylinder)


void ShapeCylinder::initClass()
{
    SO_NODE_INIT_CLASS(ShapeCylinder, ShapeRT, "ShapeRT");
}

ShapeCylinder::ShapeCylinder()
{
    SO_NODE_CONSTRUCTOR(ShapeCylinder);
    isBuiltIn = TRUE;

    SO_NODE_DEFINE_ENUM_VALUE(Caps, none);
    SO_NODE_DEFINE_ENUM_VALUE(Caps, bottom);
    SO_NODE_DEFINE_ENUM_VALUE(Caps, top);
    SO_NODE_DEFINE_ENUM_VALUE(Caps, both);

    SO_NODE_SET_SF_ENUM_TYPE(caps, Caps);
    SO_NODE_ADD_FIELD(caps, (none) );
}

ProfileRT* ShapeCylinder::getDefaultProfile() const
{
    ProfileBox* pr = new ProfileBox;
    pr->uSize.set("360d");
    return pr;
}

vec3d ShapeCylinder::getPoint(double u, double v) const
{
    return vec3d(cos(u), sin(u), v);
}

vec3d ShapeCylinder::getNormal(double u, double v) const
{
    Q_UNUSED(v)
    return vec3d(cos(u), sin(u), 0.);
}

vec3d ShapeCylinder::getDerivativeU(double u, double /*v*/) const
{
    return vec3d(-sin(u), cos(u), 0.);
}

vec3d ShapeCylinder::getDerivativeV(double /*u*/, double /*v*/) const
{
    return vec3d(0., 0., 1.);
}

vec2d ShapeCylinder::getUV(const vec3d& p) const
{
    return vec2d(atan2(p.y, p.x), p.z);
}

Box3D ShapeCylinder::getBox(ProfileRT* profile) const
{
    Box2D box = profile->getBox();
    double phiMin = box.min().x;
    double phiMax = box.max().x;
    double zMin = box.min().y;
    double zMax = box.max().y;

    double xMin = cos(phiMin);
    double xMax = cos(phiMax);
    double yMin = sin(phiMin);
    double yMax = sin(phiMax);
    if (xMin > xMax) std::swap(xMin, xMax);
    if (yMin > yMax) std::swap(yMin, yMax);

    if (phiMin <= 0. && 0. <= phiMax)
        xMax = 1.;
    if (phiMin <= -gcf::pi || phiMax >= gcf::pi)
        xMin = -1.;
    if (phiMin <= 0.5*gcf::pi && 0.5*gcf::pi <= phiMax)
        yMax = 1.;
    if (phiMin <= -0.5*gcf::pi && -0.5*gcf::pi <= phiMax)
        yMin = -1.;

    return Box3D(
        vec3d(xMin, yMin, zMin),
        vec3d(xMax, yMax, zMax)
    );
}

double ShapeCylinder::getStepHint(double u, double v) const
{
    Q_UNUSED(u)
    Q_UNUSED(v)

    //    r = {cosu, sinu, v};
    //    n = {cosu, sinu, 0};
    //    ru = {-sinu, cosu, 0};
    //    rv = {0, 0, 1};
    //    ruu = -n;
    //    ruv = 0;
    //    rvv = 0;

    //    L = ruu.n = -1;
    //    M = ruv.n = 0;
    //    N = rvv.n = 0;

    return 2*gcf::pi/48;
}

void ShapeCylinder::updateShapeGL(TShapeKit* parent)
{
    ProfileRT* profile = (ProfileRT*) parent->profileRT.getValue();
    Box2D box = profile->getBox();
    vec2d v = box.size();

    double s = v.x/(2*gcf::pi);
    if (s > 1.) s = 1.;
    int rows = 1 + ceil(48*s);

    makeQuadMesh(parent, QSize(rows, 2), true);

    // add
    if (caps.getValue() != Caps::none) {
        SoShapeKit* shapeKit = parent->m_shapeKit;
        SoCoordinate3* sCoords = (SoCoordinate3*) shapeKit->getPart("coordinate3", false);
        SoNormal* sNormals = (SoNormal*) shapeKit->getPart("normal", false);
        SoIndexedFaceSet* sMesh = (SoIndexedFaceSet*) shapeKit->getPart("shape", false);
        int iVertices = sCoords->point.getNum() - 1;
        int iFaces = sMesh->coordIndex.getNum() - 1;
        Box2D box2d = profile->getBox();
        if (caps.getValue() & Caps::top) {
            for (int r = 0; r < rows; ++r) {
                vec3d p = getPoint(2*gcf::pi*(r/(rows - 1.) - 0.5), box2d.max().y);
                sCoords->point.set1Value(++iVertices, p.x, p.y, p.z);
                sNormals->vector.set1Value(iVertices, 0, 0, 1);
                sMesh->coordIndex.set1Value(++iFaces, iVertices);
            }
            sMesh->coordIndex.set1Value(++iFaces, -1);
        }
        if (caps.getValue() & Caps::bottom) {
            for (int r = 0; r < rows; ++r) {
                vec3d p = getPoint(2*gcf::pi*(r/(rows - 1.) - 0.5), box2d.min().y);
                sCoords->point.set1Value(++iVertices, p.x, p.y, p.z);
                sNormals->vector.set1Value(iVertices, 0, 0, -1);
                sMesh->coordIndex.set1Value(++iFaces, iVertices);
            }
            sMesh->coordIndex.set1Value(++iFaces, -1);
        }
    }
}

bool ShapeCylinder::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const
{
    const vec3d& rayO = ray.origin;
    const vec3d& rayD = ray.direction();

    // |rxy|^2 = 1, r = r0 + t*d
    double A = rayD.x*rayD.x + rayD.y*rayD.y;
    double B = 2.*(rayD.x*rayO.x + rayD.y*rayO.y);
    double C = rayO.x*rayO.x + rayO.y*rayO.y - 1.;
    double ts[2];
    if (!gcf::solveQuadratic(A, B, C, &ts[0], &ts[1])) return false;

    bool ans = false;
    for (int i = 0; i < 2; ++i)
    {
        double t = ts[i];
        if (t < ray.tMin + 1e-5 || t > ray.tMax) continue;

        vec3d pHit = ray.point(t);
        vec2d uv = getUV(pHit);
        if (!profile->isInside(uv.x, uv.y)) continue;

        if (tHit == 0 && dg == 0)
            {ans = true; break;}
        else if (tHit == 0 || dg == 0)
            gcf::SevereError("ShapeCylinder::intersect");

        *tHit = t;
        dg->point = pHit;
        dg->uv = uv;
        dg->dpdu = vec3d(-pHit.y, pHit.x, 0.);
        dg->dpdv = vec3d(0., 0., 1.);
        dg->normal = vec3d(pHit.x, pHit.y, 0.);
        dg->shape = this;
        dg->isFront = dot(dg->normal, rayD) <= 0.;
        {ans = true; break;}
    }

    if (caps.getValue() != Caps::none) {
        Box2D box2d = profile->getBox();
        if (caps.getValue() & Caps::top) {
            double t = (box2d.max().y - rayO.z)*ray.invDirection().z;
            if (t < ray.tMin + 1e-5 || t > ray.tMax || (ans && t > *tHit)) {}
            else {
                vec3d pHit = ray.point(t);
                if (pHit.x*pHit.x + pHit.y*pHit.y <= 1) {
                    double phi = atan2(pHit.y, pHit.x);
                    if (phi <= box2d.max().x && phi >= box2d.min().x) {
                        *tHit = t;
                        dg->point = pHit;
                        dg->uv = vec2d(phi, box2d.max().y);
                        dg->dpdu = vec3d(1., 0., 0.);
                        dg->dpdv = vec3d(0., 1., 0.);
                        dg->normal = vec3d(0., 0., 1.);
                        dg->shape = this;
                        dg->isFront = dot(dg->normal, rayD) <= 0.;
                        ans = true;
                    }
                }
            }
        }
        if (caps.getValue() & Caps::bottom) {
            double t = (box2d.min().y - rayO.z)*ray.invDirection().z;
            if (t < ray.tMin + 1e-5 || t > ray.tMax || (ans && t > *tHit)) {}
            else {
                vec3d pHit = ray.point(t);
                if (pHit.x*pHit.x + pHit.y*pHit.y <= 1) {
                    double phi = atan2(pHit.y, pHit.x);
                    if (phi <= box2d.max().x && phi >= box2d.min().x) {
                        *tHit = t;
                        dg->point = pHit;
                        dg->uv = vec2d(phi, box2d.min().y);
                        dg->dpdu = vec3d(1., 0., 0.);
                        dg->dpdv = vec3d(0., -1., 0.);
                        dg->normal = vec3d(0., 0., -1.);
                        dg->shape = this;
                        dg->isFront = dot(dg->normal, rayD) <= 0.;
                        ans = true;
                    }
                }
            }
        }
    }

    return ans;
}
