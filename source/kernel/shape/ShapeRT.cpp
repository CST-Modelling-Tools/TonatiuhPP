#include "ShapeRT.h"

#include <QSize>
#include <QVector>

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoQuadMesh.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>

#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
//#include "kernel/profiles/ProfileRT.h"
#include "kernel/profiles/ProfileBox.h"
#include "libraries/math/3D/vec3d.h"
#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Ray.h"
#include "kernel/profiles/ProfilePolygon.h"
#include "libraries/DistMesh/PolygonMesh.h"
#include "scene/MaterialGL.h"

SO_NODE_ABSTRACT_SOURCE(ShapeRT)


void ShapeRT::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(ShapeRT, TNode, "TNode");
}

vec3d ShapeRT::getPoint(double u, double v) const
{
    return vec3d(u, v, 0.);
}

vec3d ShapeRT::getNormal(double u, double v) const
{
    Q_UNUSED(u)
    Q_UNUSED(v)
    return vec3d(0., 0., 1.);
}

vec3d ShapeRT::getDerivativeU(double u, double v) const
{
    Q_UNUSED(u)
    Q_UNUSED(v)
    return vec3d(1., 0., 0.);
}

vec3d ShapeRT::getDerivativeV(double u, double v) const
{
    Q_UNUSED(u)
    Q_UNUSED(v)
    return vec3d(0., 1., 0.);
}

#include "libraries/math/3D/Transform.h"
/*
01 11
00 10
*/
double ShapeRT::findArea(double u0, double v0, double u1, double v1, const Transform& t) const
{
    vec3d p00 = getPoint(u0, v0);
    vec3d p01 = getPoint(u0, v1);
    vec3d p10 = getPoint(u1, v0);
    vec3d p11 = getPoint(u1, v1);

    double a1 = cross(t.transformVector(p10 - p00), t.transformVector(p01 - p00)).norm();
    double a2 = cross(t.transformVector(p10 - p11), t.transformVector(p01 - p11)).norm();
    return (a1 + a2)/2;
}

//double ShapeRT::getJacobian(double u, double v) const
//{
//    Q_UNUSED(u)
//    Q_UNUSED(v)
//    return 1.;
//}

vec2d ShapeRT::getUV(const vec3d& p) const
{
    return vec2d(p.x, p.y);
}

double ShapeRT::getStepHint(double u, double v) const
{
    Q_UNUSED(u)
    Q_UNUSED(v)
    return 1.; // use infinity
}

Box3D ShapeRT::getBox(ProfileRT* profile) const
{
    Box2D box = profile->getBox();
    double zMax = 0.01*box.size().max();
    return Box3D(
        vec3d(box.min(), -zMax),
        vec3d(box.max(), zMax)
                );
}

ProfileRT* ShapeRT::getDefaultProfile() const
{
    ProfileBox* pr = new ProfileBox;
    pr->uSize = 1.;
    pr->vSize = 1.;
    return pr;
}

bool ShapeRT::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const
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

    *tHit = t;
    dg->point = pHit;
    dg->uv = vec2d(pHit.x, pHit.y);
    dg->dpdu = vec3d(1., 0., 0.);
    dg->dpdv = vec3d(0., 1., 0.);
    dg->normal = vec3d(0., 0., 1.);
    dg->shape = this;
    dg->isFront = dot(dg->normal, ray.direction()) <= 0.;
    return true;
}

struct MeshDensityShape: public MeshDensity
{
    virtual double operator()(double u, double v) {return shape->getStepHint(u, v);}
    ShapeRT* shape;
};

void ShapeRT::makeQuadMesh(TShapeKit* parent, const QSize& dims, bool forceIndexed)
{
    MaterialGL* mGL = (MaterialGL*) parent->material.getValue();
    bool reverseNormals = mGL->reverseNormals.getValue();
    ProfileRT* profile = (ProfileRT*) parent->profileRT.getValue();
    SoShapeKit* shapeKit = parent->m_shapeKit;
    shapeKit->setPart("normalBinding", new SoNormalBinding);

    if (ProfilePolygon* profilePolygon = dynamic_cast<ProfilePolygon*>(profile))
    {
        // call
        const QPolygonF& qpolygon = profilePolygon->getPolygon();
//        QSizeF rect = qpolygon.boundingRect().size();
//        double s = std::min(rect.width()/(dims.width() - 1), rect.height()/(dims.height() - 1));

        MeshDensityShape mds;
        mds.shape = (ShapeRT*) parent->shapeRT.getValue();


        PolygonMesh polygonMesh(qpolygon);
//        if (!polygonMesh.makeMesh(s))
//            return;
        if (!polygonMesh.makeMesh(1e8, mds))
            return;

        // fill
        QVector<SbVec3f> vertices;
        QVector<SbVec3f> normals;
        for (const vec2d& uv : polygonMesh.getPoints()) {
            vec3d point = getPoint(uv.x, uv.y);
            vec3d normal = getNormal(uv.x, uv.y);
            if (reverseNormals) normal = -normal;
            vertices << SbVec3f(point.x, point.y, point.z);
            normals << SbVec3f(normal.x, normal.y, normal.z);
        }

        QVector<int> faces;
        for (const auto& tri : polygonMesh.getTriangles()) {
            faces << tri.a;
            faces << tri.b;
            faces << tri.c;
            faces << SO_END_FACE_INDEX;
        }

        SoCoordinate3* sVertices = new SoCoordinate3;
        sVertices->point.setValues(0, vertices.size(), vertices.data());
        shapeKit->setPart("coordinate3", sVertices);

        SoNormal* sNormals = new SoNormal;
        sNormals->vector.setValues(0, normals.size(), normals.data());
        shapeKit->setPart("normal", sNormals);

        SoIndexedFaceSet* sMesh = new SoIndexedFaceSet;
        sMesh->coordIndex.setValues(0, faces.size(), faces.data());
        shapeKit->setPart("shape", sMesh);
    }
    else
    {
        QSize dimensions = dims;
        QVector<vec2d> uvs = profile->makeMesh(dimensions);

        QVector<SbVec3f> vertices;
        QVector<SbVec3f> normals;
        for (const vec2d& uv : uvs) {
            vec3d point = getPoint(uv.x, uv.y);
            vec3d normal = getNormal(uv.x, uv.y);
            if (reverseNormals) normal = -normal;
            vertices << SbVec3f(point.x, point.y, point.z);
            normals << SbVec3f(normal.x, normal.y, normal.z);
        }

        SoCoordinate3* sVertices = new SoCoordinate3;
        sVertices->point.setValues(0, vertices.size(), vertices.data());
        shapeKit->setPart("coordinate3", sVertices);

        SoNormal* sNormals = new SoNormal;
        sNormals->vector.setValues(0, normals.size(), normals.data());
        shapeKit->setPart("normal", sNormals);

        if (!forceIndexed) {
            SoQuadMesh* sMesh = new SoQuadMesh;
            sMesh->verticesPerRow = dimensions.height();
            sMesh->verticesPerColumn = dimensions.width();
            shapeKit->setPart("shape", sMesh);
        }
        else {
            QVector<int> faces;
            for (int n = 0; n < dimensions.width() - 1; n++)
                for (int m = 0; m < dimensions.height() - 1; ++m) {
                    int iA = n * dimensions.height() + m;
                    int iB = iA + dimensions.height();
                    int iC = iB + 1;
                    int iD = iA + 1;
                    faces << iA << iD << iC << iB << -1;
                }
            SoIndexedFaceSet* sMesh = new SoIndexedFaceSet;
            sMesh->coordIndex.setValues(0, faces.size(), faces.data());
            shapeKit->setPart("shape", sMesh);
        }
    }
}
