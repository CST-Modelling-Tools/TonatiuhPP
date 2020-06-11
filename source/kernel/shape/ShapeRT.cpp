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
#include "libraries/DistMesh/distmesh.h"

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
    if (ProfilePolygon* porfilePolygon = dynamic_cast<ProfilePolygon*>(profile))
    {
        const QPolygonF& qpolygon = porfilePolygon->getPolygon();
        Eigen::ArrayXXd polygon(qpolygon.size(), 2);
        for (int n = 0; n < qpolygon.size(); ++n) {
            polygon(n, 0) = qpolygon[n].x();
            polygon(n, 1) = qpolygon[n].y();
        }

        QSizeF rect = qpolygon.boundingRect().size();
        double s = std::min(rect.width()/(dims.width() - 1), rect.height()/(dims.height() - 1));

        for (int n = 1; n < qpolygon.size(); ++n) {
            QPointF ed = qpolygon[n] - qpolygon[n - 1];
            double se = 0.3*sqrt(ed.x()*ed.x() + ed.y()*ed.y());
            if (se < s) s = se;
        }

        Eigen::ArrayXXd points;
        Eigen::ArrayXXi elements;
        std::tie(points, elements) = distmesh::distmesh(
            distmesh::distanceFunction::polygon(polygon),
            s,
            1.0,
            distmesh::utils::boundingBox(2),
            polygon);

        QVector<SbVec3f> vertices;
        QVector<SbVec3f> normals;
        for (int n = 0; n < points.rows(); ++n) {
            double u = points(n, 0);
            double v = points(n, 1);
            Vector3D point = getPoint(u, v);
            Vector3D normal = getNormal(u, v);
            if (reverseNormals) normal = -normal;
            vertices << SbVec3f(point.x, point.y, point.z);
            normals << SbVec3f(normal.x, normal.y, normal.z);
        }

        QVector<int> indices;
        for (int n = 0; n < elements.rows(); ++n) {
            for (int m = 0; m < elements.cols(); ++m)
                indices <<  elements(n, m);
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
        QVector<Vector2D> uvs = profile->makeMesh(dims);

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
        sMesh->verticesPerRow = dims.height();
        sMesh->verticesPerColumn = dims.width();

        parent->setPart("shape", sMesh);
    }
}
