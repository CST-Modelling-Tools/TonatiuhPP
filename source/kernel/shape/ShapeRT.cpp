#include "ShapeRT.h"

#include <QSize>
#include <QVector>

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoQuadMesh.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>

#include "kernel/scene/TShapeKit.h"
#include "kernel/profiles/ProfileRT.h"
#include "libraries/geometry/Vector3D.h"
#include "libraries/geometry/Box3D.h"
#include "kernel/profiles/ProfilePolygon.h"
#include "libraries/DistMesh/PolygonMesh.h"

SO_NODE_ABSTRACT_SOURCE(ShapeRT)


void ShapeRT::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(ShapeRT, SoNode, "Node");
}

Box3D ShapeRT::getBox(ProfileRT* aperture) const
{
    Q_UNUSED(aperture)
    return Box3D::UnitCube;
}

Vector3D ShapeRT::getPoint(double u, double v) const
{
    Q_UNUSED(u)
    Q_UNUSED(v)
    return Vector3D(0., 0., 0.);
}

Vector3D ShapeRT::getNormal(double u, double v) const
{
    Q_UNUSED(u)
    Q_UNUSED(v)
    return Vector3D(0., 0., 1.);
}

void ShapeRT::makeQuadMesh(TShapeKit* parent, const QSize& dims, bool reverseNormals, bool reverseClock)
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
        for (const Vector2D& uv : polygonMesh.getPoints()) {
            Vector3D point = getPoint(uv.x, uv.y);
            Vector3D normal = getNormal(uv.x, uv.y);
            if (reverseNormals) normal = -normal;
            vertices << SbVec3f(point.x, point.y, point.z);
            normals << SbVec3f(normal.x, normal.y, normal.z);
        }

        QVector<int> indices;
        for (const auto& tri : polygonMesh.getTriangles()) {
            indices << tri.a;
            indices << tri.b;
            indices << tri.c;
            indices << SO_END_FACE_INDEX;
        }

        SoCoordinate3* sVertices = new SoCoordinate3;
        sVertices->point.setValues(0, vertices.size(), vertices.data());
        parent->setPart("coordinate3", sVertices);

        SoNormal* sNormals = new SoNormal;
        sNormals->vector.setValues(0, normals.size(), normals.data());
        parent->setPart("normal", sNormals);


        SoIndexedFaceSet* sMesh = new SoIndexedFaceSet;
        sMesh->coordIndex.setValues(0, indices.size(), indices.data());

        parent->setPart("shape", sMesh);
    }
    else
    {
        QSize dimensions = dims;
        QVector<Vector2D> uvs = profile->makeMesh(dimensions);

        QVector<SbVec3f> vertices;
        QVector<SbVec3f> normals;
        for (const Vector2D& uv : uvs) {
            Vector3D point = getPoint(uv.x, uv.y);
            Vector3D normal = getNormal(uv.x, uv.y);
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
