#include "ShapeRT.h"

#include <QSize>
#include <QVector>

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoQuadMesh.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>

#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "kernel/profiles/ProfileRT.h"
#include "libraries/math/3D/vec3d.h"
#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Ray.h"
#include "kernel/profiles/ProfilePolygon.h"
#include "libraries/DistMesh/PolygonMesh.h"

SO_NODE_ABSTRACT_SOURCE(ShapeRT)


void ShapeRT::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(ShapeRT, SoNode, "Node");
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

vec2d ShapeRT::getUV(const vec3d& p) const
{
    return vec2d(p.x, p.y);
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
    dg->u = pHit.x;
    dg->v = pHit.y;
    dg->dpdu = vec3d(1., 0., 0.);
    dg->dpdv = vec3d(0., 1., 0.);
    dg->normal = vec3d(0., 0., 1.);
    dg->shape = this;
    dg->isFront = dot(dg->normal, ray.direction()) <= 0.;
    return true;
}

void ShapeRT::makeQuadMesh(TShapeKit* parent, const QSize& dims, bool reverseNormals, bool /*reverseClock*/)
{
    ProfileRT* profile = (ProfileRT*) parent->profileRT.getValue();
    if (ProfilePolygon* profilePolygon = dynamic_cast<ProfilePolygon*>(profile))
    {
        const QPolygonF& qpolygon = profilePolygon->getPolygon();
        QSizeF rect = qpolygon.boundingRect().size();
        double s = std::min(rect.width()/(dims.width() - 1), rect.height()/(dims.height() - 1));

        for (int n = 1; n < qpolygon.size(); ++n) {
            QPointF ed = qpolygon[n] - qpolygon[n - 1];
            double se = 0.3*sqrt(ed.x()*ed.x() + ed.y()*ed.y());
            if (se < s) s = se;
        }

        PolygonMesh polygonMesh(qpolygon);
        polygonMesh.makeMesh(s);

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
        parent->setPart("coordinate3", sVertices);

        SoNormal* sNormals = new SoNormal;
        sNormals->vector.setValues(0, normals.size(), normals.data());
        parent->setPart("normal", sNormals);


        SoIndexedFaceSet* sMesh = new SoIndexedFaceSet;
        sMesh->coordIndex.setValues(0, faces.size(), faces.data());

        parent->setPart("shape", sMesh);
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
        parent->setPart("coordinate3", sVertices);

        SoNormal* sNormals = new SoNormal;
        sNormals->vector.setValues(0, normals.size(), normals.data());
        parent->setPart("normal", sNormals);

        SoQuadMesh* sMesh = new SoQuadMesh;
        sMesh->verticesPerRow = dimensions.height();
        sMesh->verticesPerColumn = dimensions.width();

        parent->setPart("shape", sMesh);
    }
}
