#include "ShapeRT.h"

#include <QSize>
#include <QVector>

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoQuadMesh.h>

#include "kernel/scene/TShapeKit.h"
#include "kernel/profiles/ProfileRT.h"
#include "libraries/geometry/Vector3D.h"
#include "libraries/geometry/Box3D.h"
#include "kernel/profiles/ProfilePolygon.h"

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
    return Vector3D(0, 0, 0);
}

Vector3D ShapeRT::getNormal(double u, double v) const
{
    return Vector3D(0, 0, 0);
}

void ShapeRT::makeQuadMesh(TShapeKit* parent, const QSize& dims, bool reverseNormals, bool reverseClock)
{
    ProfileRT* profile = (ProfileRT*) parent->profileRT.getValue();
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

    SoNormal* sNormals = new SoNormal;
    sNormals->vector.setValues(0, normals.size(), normals.data());

    parent->setPart("coordinate3", sVertices);
    parent->setPart("normal", sNormals);

    SoQuadMesh* sMesh = new SoQuadMesh;
    sMesh->verticesPerRow = dims.height();
    sMesh->verticesPerColumn = dims.width();

    parent->setPart("shape", sMesh);

//    if (ProfilePolygon* pp = dynamic_cast<ProfilePolygon*>(profile))
//    {
//        parent->setPart('profile', );
//    }
}
