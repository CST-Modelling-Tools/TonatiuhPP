#include "SunAperture.h"

#include <QString>
#include <QVector>
#include <QImage>
#include <QBrush>
#include <QColor>
#include <QImage>
#include <QPaintEngine>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>

#include <math.h>
#include "libraries/math/3D/Box3D.h"
#include "libraries/math/3D/Ray.h"
#include "libraries/math/3D/Transform.h"
#include "libraries/math/3D/vec3d.h"
#include "libraries/math/gcf.h"
#include "scene/TShapeKit.h"
#include "shape/DifferentialGeometry.h"
#include "SunKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/profiles/ProfileRT.h"


SO_NODE_SOURCE(SunAperture)

void SunAperture::initClass()
{
    SO_NODE_INIT_CLASS(SunAperture, SoShape, "Shape");
}

SunAperture::SunAperture():
    m_xCells(0),
    m_yCells(0)
{
    SO_NODE_CONSTRUCTOR(SunAperture);
    SO_NODE_ADD_FIELD( disabledNodes, ("") );

    m_xMin = -0.5;
    m_xMax = 0.5;
    m_yMin = -0.5;
    m_yMax = 0.5;
    m_delta = 100.;
}

SunAperture::~SunAperture()
{

}

double SunAperture::getArea() const
{
    double dx = (m_xMax - m_xMin)/m_xCells;
    double dy = (m_yMax - m_yMin)/m_yCells;
    return dx*dy*m_cells.size();
}

/*!
 * Returns the indexes of the valid areas to the ray tracer.
 */
vec3d SunAperture::Sample(double u, double v, int w, int h) const //? w <> h
{
    if (u < 0. || u > 1. || v < 0. || v > 1.)
        gcf::SevereError("Function SunAperture::GetPoint3D called with invalid parameters");

    double xWidth = (m_xMax - m_xMin)/m_xCells;
    double yWidth = (m_yMax - m_yMin)/m_yCells;

    double x = m_xMin + (w + u)*xWidth;
    double y = m_yMin + (h + v)*yWidth;

    return vec3d(x, y, 0.);
}

void SunAperture::setSize(double xMin, double xMax, double yMin, double yMax, double delta)
{
    m_xMin = xMin - delta;
    m_xMax = xMax + delta;
    m_yMin = yMin - delta;
    m_yMax = yMax + delta;
    m_delta = delta;

//    disabledNodes = disabledNodes.getValue(); // character 0 lost
    disabledNodes = disabledNodes.getValue().getString(); // for update
}

void SunAperture::findTexture(int xPixels, int yPixels, QVector<QPair<TShapeKit*, Transform> > surfaces, SunKit* sunKit)
{
    double xWidth = m_xMax - m_xMin;
    double yWidth = m_yMax - m_yMin;

    while (xWidth / xPixels < m_delta) xPixels--;
    double xStep = xWidth/xPixels;

    while (yWidth / yPixels < m_delta) yPixels--;
    double yStep = yWidth/yPixels;

    QImage* image = new QImage(xPixels, yPixels, QImage::Format_Grayscale8);
    image->fill(Qt::black);

    QPainter painter(image);

    QBrush brush(Qt::white);
    painter.setBrush(brush);

    QPen pen(Qt::white);
    painter.setPen(pen);

    painter.setRenderHint(QPainter::Antialiasing, false);

    for (const QPair<TShapeKit*, Transform>& s : surfaces)
    {
        ShapeRT* shape = static_cast<ShapeRT*>(s.first->shapeRT.getValue());
        if (!shape) continue;
        ProfileRT* aperture = static_cast<ProfileRT*>(s.first->profileRT.getValue());
        if (!aperture) continue;
        Box3D box = shape->getBox(aperture);

        QVector<vec3d> ps;
        ps << vec3d(box.pMin.x, box.pMin.y, box.pMin.z);
        ps << vec3d(box.pMin.x, box.pMin.y, box.pMax.z);
        ps << vec3d(box.pMin.x, box.pMax.y, box.pMin.z);
        ps << vec3d(box.pMin.x, box.pMax.y, box.pMax.z);
        ps << vec3d(box.pMax.x, box.pMin.y, box.pMin.z);
        ps << vec3d(box.pMax.x, box.pMin.y, box.pMax.z);
        ps << vec3d(box.pMax.x, box.pMax.y, box.pMin.z);
        ps << vec3d(box.pMax.x, box.pMax.y, box.pMax.z);

        QVector<QPointF> qps;
        for (vec3d& p : ps) {
            p = s.second.transformPoint(p);
            QPointF q((p.x - m_xMin)/xStep, (p.y - m_yMin)/yStep);
            qps << q;
            painter.drawPoint(q); // for polygons smaller than pixel
        }

        painter.drawPolygon(QPolygonF({qps[0], qps[1], qps[3], qps[2]})); // -x
        painter.drawPolygon(QPolygonF({qps[4], qps[5], qps[7], qps[6]})); // +x
        painter.drawPolygon(QPolygonF({qps[0], qps[4], qps[5], qps[1]})); // -y
        painter.drawPolygon(QPolygonF({qps[2], qps[6], qps[7], qps[3]})); // +y
        painter.drawPolygon(QPolygonF({qps[0], qps[4], qps[6], qps[2]})); // -z
        painter.drawPolygon(QPolygonF({qps[5], qps[1], qps[3], qps[7]})); // +z
    }

    int** areaMatrix = new int*[yPixels];
    for (int i = 0; i < yPixels; i++)
        areaMatrix[i] = new int[xPixels];

    QVector<uchar> bmp;
    bmp.resize(xPixels*yPixels);

    QRgb black = qRgb(0, 0, 0);
    for (int i = 0; i < xPixels; ++i)
    {
        for (int j = 0; j < yPixels; ++j)
        {
            int v = 0;
            for (int qi = i - 1; qi <= i + 1; ++qi)
                for (int qj = j - 1; qj <= j + 1; ++qj)
                {
                    if (qi < 0 || qi >= xPixels) continue;
                    if (qj < 0 || qj >= yPixels) continue;
                    v += image->pixel(qi, qj) == black ? 0 : 1;
                }
            bmp[i*yPixels + j] = v > 0 ? 255 : 0;
        }
    }

    m_xCells = xPixels;
    m_yCells = yPixels;

    m_cells.clear();

    for (int i = 0; i < m_xCells; ++i)
        for (int j = 0; j < m_yCells; ++j)
            if (bmp[i*m_yCells + j] > 0)
                m_cells.push_back(QPair<int, int>(i, j));

    Q_UNUSED(sunKit)
//    QVector<uchar> bmpTr;
//    bmpTr.resize(xPixels*yPixels);
//    for (int i = 0; i < xPixels; ++i)
//        for (int j = 0; j < yPixels; ++j)
//            bmpTr[j*xPixels + i] = bmp[i*yPixels + j];

//    SoTexture2* texture = static_cast<SoTexture2*>(sunKit->getPart("iconTexture", true));
//    texture->image.setValue(SbVec2s(xPixels, yPixels), 1, bmpTr.data()); // 1 is for gray
//    texture->wrapS = SoTexture2::CLAMP;
//    texture->wrapT = SoTexture2::CLAMP;
}

void SunAperture::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/)
{
    box.setBounds(
        SbVec3f(m_xMin, m_yMin, 0.),
        SbVec3f(m_xMax, m_yMax, 0.)
    );
}

void SunAperture::generatePrimitives(SoAction* action)
{
    SoPrimitiveVertex pv;
    SbVec4f texCoord;
    SbVec3f normal(0., 0., -1.);

    beginShape(action, TRIANGLE_STRIP);

    // P0
    SbVec3f point(m_xMin, m_yMin, 0.);
    pv.setPoint(point);
    pv.setNormal(normal);
    texCoord = SbVec4f(0., 0., 0., 1.);
    pv.setTextureCoords(texCoord);
    shapeVertex(&pv);

    // P1
    point = SbVec3f(m_xMin, m_yMax, 0.);
    pv.setPoint(point);
    pv.setNormal(normal);
    texCoord = SbVec4f(0., 1., 0., 1.);
    pv.setTextureCoords(texCoord);
    shapeVertex(&pv);

    // P2
    point = SbVec3f(m_xMax, m_yMin, 0.);
    pv.setPoint(point);
    pv.setNormal(normal);
    texCoord = SbVec4f(1., 0., 0., 1.);
    pv.setTextureCoords(texCoord);
    shapeVertex(&pv);

    // P3
    point = SbVec3f(m_xMax, m_yMax, 0.);
    pv.setPoint(point);
    pv.setNormal(normal);
    texCoord = SbVec4f(1., 1., 0., 1.);
    pv.setTextureCoords(texCoord);
    shapeVertex(&pv);

    endShape();
}
