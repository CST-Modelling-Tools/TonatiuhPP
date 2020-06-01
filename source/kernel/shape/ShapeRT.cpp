#include "ShapeRT.h"

#include <QSize>
#include <QVector>

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoQuadMesh.h>

#include "kernel/scene/TShapeKit.h"
#include "libraries/geometry/Vector3D.h"
#include "libraries/geometry/BoundingBox.h"


SO_NODE_ABSTRACT_SOURCE(ShapeRT)


void ShapeRT::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(ShapeRT, SoNode, "Node");
}

void ShapeRT::computeBBox(SoAction* action, SbBox3f& box, SbVec3f& center)
{
    Q_UNUSED(action)
    Q_UNUSED(center)

    BoundingBox b = getBox();
    SbVec3f min(b.pMin.x, b.pMin.y, b.pMin.z);
    SbVec3f max(b.pMax.x, b.pMax.y, b.pMax.z);
    box.setBounds(min, max);

//    center.setValue(0., 0., 0.);
}

void ShapeRT::makeQuadMesh(TShapeKit* parent, const QSize& dims, bool reverseNormals, bool reverseClock)
{
    const int iMax = dims.width(); // u
    const int jMax = dims.height(); // v
    QVector<SbVec3f> vertices;
    QVector<SbVec3f> normals;

    for (int i = 0; i < iMax; ++i) {
        double u = i/double(iMax - 1);
        for (int j = 0; j < jMax; ++j) {
            double v = j/double(jMax - 1);

            Vector3D point = getPoint(u, v);
            Vector3D normal = getNormal(u, v);
            if (reverseNormals) normal = -normal;

            vertices << SbVec3f(point.x, point.y, point.z);
            normals << SbVec3f(normal.x, normal.y, normal.z);
        }
    }

    SoCoordinate3* sVertices = new SoCoordinate3;
    sVertices->point.setValues(0, vertices.size(), vertices.data());

    SoNormal* sNormals = new SoNormal;
    sNormals->vector.setValues(0, normals.size(), normals.data());

    parent->setPart("coordinate3", sVertices);
    parent->setPart("normal", sNormals);

    SoQuadMesh* sMesh = new SoQuadMesh;
    sMesh->verticesPerRow = jMax;
    sMesh->verticesPerColumn = iMax;

    parent->setPart("shape", sMesh);
}
