#include "SunAperture.h"

#include <QString>
#include <QVector>
#include <QImage>
#include <QBrush>
#include <QColor>
#include <QImage>
#include <QPaintEngine>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>

#include <math.h>
#include "libraries/geometry/BoundingBox.h"
#include "libraries/geometry/Ray.h"
#include "libraries/geometry/Transform.h"
#include "libraries/geometry/Vector3D.h"
#include "libraries/geometry/gcf.h"
#include "scene/TShapeKit.h"
#include "shape/DifferentialGeometry.h"


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
Vector3D SunAperture::Sample(double u, double v, int w, int h) const //? w <> h
{
    if (u < 0. || u > 1. || v < 0. || v > 1.)
        gcf::SevereError("Function SunAperture::GetPoint3D called with invalid parameters");

    double xWidth = (m_xMax - m_xMin)/m_xCells;
    double yWidth = (m_yMax - m_yMin)/m_yCells;

    double x = m_xMin + (w + u)*xWidth;
    double y = m_yMin + (h + v)*yWidth;

    return Vector3D(x, y, 0.);
}

void SunAperture::setSize(double xMin, double xMax, double yMin, double yMax, double delta)
{
    m_xMin = xMin - delta;
    m_xMax = xMax + delta;
    m_yMin = yMin - delta;
    m_yMax = yMax + delta;
    m_delta = delta;
    disabledNodes = disabledNodes.getValue();
}

void SunAperture::findTexture(int xPixels, int yPixels, QVector<QPair<TShapeKit*, Transform> > surfacesList)
{
    double xWidth = m_xMax - m_xMin;
    double yWidth = m_yMax - m_yMin;

    while (xWidth / xPixels < m_delta) xPixels--;
    double xWidthPixel = xWidth/xPixels;

    while (yWidth / yPixels < m_delta) yPixels--;
    double yWidthPixel = yWidth/yPixels;

    QImage* image = new QImage(xPixels, yPixels, QImage::Format_RGB32);
    image->setOffset(QPoint(0.5, 0.5));
    image->fill(Qt::white);

    QPainter painter(image);

    QBrush brush(Qt::black);
    painter.setBrush(brush);

    QPen pen(Qt::black);
    painter.setPen(pen);

    //painter.setRenderHint(QPainter::Antialiasing);

    for (const auto& s : surfacesList)
    {
        ShapeAbstract* shapeNode = static_cast<ShapeAbstract*>(s.first->getPart("shape", false));
        if (!shapeNode) continue;
        BoundingBox box = shapeNode->getBox();

        QVector<Vector3D> ps;
        ps << Vector3D(box.pMin.x, box.pMin.y, box.pMin.z);
        ps << Vector3D(box.pMax.x, box.pMin.y, box.pMin.z);
        ps << Vector3D(box.pMax.x, box.pMin.y, box.pMax.z);
        ps << Vector3D(box.pMin.x, box.pMin.y, box.pMax.z);
        ps << Vector3D(box.pMin.x, box.pMax.y, box.pMin.z);
        ps << Vector3D(box.pMax.x, box.pMax.y, box.pMin.z);
        ps << Vector3D(box.pMax.x, box.pMax.y, box.pMax.z);
        ps << Vector3D(box.pMin.x, box.pMax.y, box.pMax.z);

        QVector<QPointF> qps;
        for (Vector3D& p : ps) {
            p = s.second.transformPoint(p);
            qps << QPointF((p.x - m_xMin)/xWidthPixel, (p.y - m_yMin)/yWidthPixel);
        }

        painter.drawPolygon(QPolygonF({qps[0], qps[1], qps[2], qps[3]}));
        painter.drawPolygon(QPolygonF({qps[0], qps[1], qps[5], qps[4]}));
        painter.drawPolygon(QPolygonF({qps[0], qps[3], qps[7], qps[4]}));
        painter.drawPolygon(QPolygonF({qps[1], qps[2], qps[6], qps[5]}));
        painter.drawPolygon(QPolygonF({qps[2], qps[3], qps[7], qps[6]}));
        painter.drawPolygon(QPolygonF({qps[4], qps[5], qps[6], qps[7]}));
    }

    int** areaMatrix = new int*[yPixels];
    for (int i = 0; i < yPixels; i++)
        areaMatrix[i] = new int[xPixels];

    QVector<uchar> bmp;
    bmp.resize(xPixels*yPixels);

    QRgb black = qRgb(0, 0, 0);

    for (int i = 0; i < xPixels; i++)
    {
        for (int j = 0; j < yPixels; j++)
        {
            int v = 0;
            for (int qi = i - 1; qi <= i + 1; ++qi)
                for (int qj = j - 1; qj <= j + 1; ++qj)
                {
                    if (qi < 0 || qi >= xPixels) continue;
                    if (qj < 0 || qj >= yPixels) continue;
                    v += image->pixel(qi, qj) == black ? 1 : 0;
                }
            bmp[i*yPixels + j] = v > 0 ? 0 : 255;
        }
    }

//    SoTexture2* texture = static_cast< SoTexture2* >(getPart("iconTexture", true) );
//    texture->image.setValue(SbVec2s(heightPixeles, widthPixeles), 1, bitmap);
//    texture->wrapS = SoTexture2::CLAMP;
//    texture->wrapT = SoTexture2::CLAMP;

    SetLightSourceArea(xPixels, yPixels, bmp);
}

void SunAperture::SetLightSourceArea(int w, int h, QVector<uchar> bmp)
{
    m_xCells = w;
    m_yCells = h;

    m_cells.clear();

    for (int i = 0; i < m_xCells; ++i)
        for (int j = 0; j < m_yCells; ++j)
            if (bmp[i*h + j] == 0)
                m_cells.push_back(QPair<int, int>(i, j));
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

    SoState* state = action->getState();
    SbBool useTexFunc = (SoTextureCoordinateElement::getType(state) == SoTextureCoordinateElement::FUNCTION);
    const SoTextureCoordinateElement* tce = 0;
    if (useTexFunc)
        tce = SoTextureCoordinateElement::getInstance(state);

    beginShape(action, TRIANGLE_STRIP);

    // P1
    SbVec3f point(m_xMin, m_yMax, 0.);
    pv.setPoint(point);
    pv.setNormal(normal);
    if (useTexFunc)
        texCoord = tce->get(point, normal);
    else
        texCoord = SbVec4f(0., 1., 0., 1.);
    pv.setTextureCoords(texCoord);
    shapeVertex(&pv);

    // P2
    point = SbVec3f( m_xMin, m_yMin, 0.);
    pv.setPoint(point);
    pv.setNormal(normal);
    if (useTexFunc)
        texCoord = tce->get(point, normal);
    else
        texCoord = SbVec4f(0., 0., 0., 1.);
    pv.setTextureCoords(texCoord);
    shapeVertex(&pv);

    // P3
    point = SbVec3f(m_xMax, m_yMax, 0.);
    pv.setPoint(point);
    pv.setNormal(normal);
    if (useTexFunc)
        texCoord = tce->get(point, normal);
    else
        texCoord = SbVec4f(1., 1., 0., 1.);
    pv.setTextureCoords(texCoord);
    shapeVertex(&pv);

    // P4
    point = SbVec3f(m_xMax, m_yMin, 0.);
    pv.setPoint(point);
    pv.setNormal(normal);
    if (useTexFunc)
        texCoord = tce->get(point, normal);
    else
        texCoord = SbVec4f(1., 0., 0., 1.);
    pv.setTextureCoords(texCoord);
    shapeVertex(&pv);

    endShape();
}
