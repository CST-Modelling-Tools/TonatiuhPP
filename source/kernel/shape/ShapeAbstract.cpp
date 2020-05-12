#include "ShapeAbstract.h"

#include "libraries/geometry/Point3D.h"
#include "libraries/geometry/Vector3D.h"
#include "libraries/geometry/BBox.h"
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <QSize>
#include <QVector>


SO_NODE_ABSTRACT_SOURCE(ShapeAbstract)


void ShapeAbstract::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(ShapeAbstract, SoShape, "Shape");
}

bool ShapeAbstract::intersectP(const Ray& ray) const
{
     return intersect(ray, 0, 0);
}

bool ShapeAbstract::isInside(double u, double v) const
{
    return u < 0. || u > 1. || v < 0. || v > 1.;
}

void ShapeAbstract::computeBBox(SoAction* action, SbBox3f& box, SbVec3f& center)
{
    Q_UNUSED(action)
    Q_UNUSED(center)

    BoundingBox b = getBox();
    SbVec3f min(b.pMin.x, b.pMin.y, b.pMin.z);
    SbVec3f max(b.pMax.x, b.pMax.y, b.pMax.z);
    box.setBounds(min, max);

//    center.setValue(0., 0., 0.);
}

void ShapeAbstract::generateQuads(SoAction* action, const QSize& dims, bool reverseNormals, bool reverseClock)
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

    const SoTextureCoordinateElement* tce = 0;
    SoState* state = action->getState();
    SbBool useTexFunc = (SoTextureCoordinateElement::getType(state) ==
        SoTextureCoordinateElement::FUNCTION);
    if (useTexFunc) tce = SoTextureCoordinateElement::getInstance(state);

    beginShape(action, QUADS);
    SoPrimitiveVertex pv;
    for (int i = 0; i < iMax - 1; ++i)
        for (int j = 0; j < jMax - 1; ++j) {
            int quad[] =  {
                i*jMax + j,
                i*jMax + j + 1,
                (i + 1)*jMax + j + 1,
                (i + 1)*jMax + j
            };
            for (int k = 0; k < 4; ++k) {
                int q = reverseClock ? quad[3 - k] : quad[k];
                pv.setPoint(vertices[q]);
                pv.setNormal(normals[q]);
                SbVec4f texCoord = useTexFunc ? tce->get(pv.getPoint(), pv.getNormal()): SbVec4f(1., 1., 0., 1.);
                pv.setTextureCoords(texCoord);
                shapeVertex(&pv);
            }
        }
    endShape();
}
