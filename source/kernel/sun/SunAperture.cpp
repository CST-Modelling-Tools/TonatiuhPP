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
    m_heightElements(0),
    m_lightAreaMatrix(0),
    m_widthElements(0)
{
    SO_NODE_CONSTRUCTOR(SunAperture);
    m_xMin = -0.5;
    m_xMax = 0.5;
    m_yMin = -0.5;
    m_yMax = 0.5;
    m_delta = 100.;
}

SunAperture::~SunAperture()
{
    for (int i = 0; i < m_heightElements; i++)
        delete[] m_lightAreaMatrix[i];
    delete[] m_lightAreaMatrix;
}

double SunAperture::getArea() const
{
    double cellWidth = (m_xMax - m_xMin)/m_widthElements;
    double cellHeight = (m_yMax - m_yMin)/m_heightElements;
    return cellWidth*cellHeight*m_cells.size();
}

/*!
 * Returns the indexes of the valid areas to the ray tracer.
 */
Vector3D SunAperture::Sample(double u, double v, int h, int w) const //? w <> h
{
    if ( OutOfRange(u, v) )
        gcf::SevereError("Function SunAperture::GetPoint3D called with invalid parameters" );

    // size of cells the sun is divided
    double xWidth = (m_xMax - m_xMin)/m_widthElements;
    double yWidth = (m_yMax - m_yMin)/m_heightElements;

    // calculate the photon coordinate
    double x = m_xMin + (u + w)*xWidth;
    double y = m_yMin + (v + h)*yWidth;

    return Vector3D(x, y, 0.);
}

void SunAperture::setSize(double xMin, double xMax, double yMin, double yMax, double delta)
{
    m_xMin = xMin - delta;
    m_xMax = xMax + delta;
    m_yMin = yMin - delta;
    m_yMax = yMax + delta;
    m_delta = delta;
}

void SunAperture::SetLightSourceArea(int h, int w, int** lightArea)
{
    if (m_lightAreaMatrix)
    {
        for (int i = 0; i < m_heightElements; i++ )
            delete[] m_lightAreaMatrix[i];
        delete[] m_lightAreaMatrix;
    }

    m_heightElements = h;
    m_widthElements = w;
    m_lightAreaMatrix = lightArea;

    m_cells.clear();

    for( int i = 0; i < m_heightElements; i++ )
        for( int j = 0; j < m_widthElements; j++ )
            if( m_lightAreaMatrix[i][j] == 1 )
                m_cells.push_back( QPair< int, int >( i, j ) );

}

void SunAperture::findTexture(int xPixels, int yPixels, QVector<QPair<TShapeKit*, Transform> > surfacesList)
{
    double xWidth = m_xMax - m_xMin;
    double yWidth = m_yMax - m_yMin;

    while (xWidth / xPixels < m_delta) xPixels--;
    double xWidthPixel = xWidth/xPixels;

    while (yWidth / yPixels < m_delta) yPixels--;
    double yWidthPixel = yWidth/yPixels;

    QImage* sourceImage = new QImage(xPixels, yPixels, QImage::Format_RGB32);
    sourceImage->setOffset(QPoint(0.5, 0.5));
    sourceImage->fill(Qt::white);

    QPainter painter(sourceImage);

    QBrush brush(Qt::black);
    painter.setBrush(brush);

    QPen pen(Qt::black);
    painter.setPen(pen);

    //painter.setRenderHint(   QPainter::Antialiasing);

    //QPen pen( Qt::black, Qt::NoPen );
    //painter.setPen( pen );

    for (int s = 0; s < surfacesList.size(); s++)
    {
        TShapeKit* surfaceKit = surfacesList[s].first;
        Transform surfaceTransform = surfacesList[s].second;

        ShapeAbstract* shapeNode = static_cast<ShapeAbstract*>(surfaceKit->getPart("shape", false));
        if (shapeNode)
        {
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
                p = surfaceTransform.transformPoint(p);
                qps <<  QPoint((p.x - m_xMin)/xWidthPixel, (p.y - m_yMin)/yWidthPixel);
            }

            QPolygonF polygon1({qps[0], qps[1], qps[2], qps[3]});
            QPolygonF polygon2({qps[0], qps[1], qps[5], qps[4]});
            QPolygonF polygon3({qps[0], qps[3], qps[7], qps[4]});
            QPolygonF polygon4({qps[1], qps[2], qps[6], qps[5]});
            QPolygonF polygon5({qps[2], qps[3], qps[7], qps[6]});
            QPolygonF polygon6({qps[4], qps[5], qps[6], qps[7]});

            painter.drawPolygon(polygon1);
            painter.drawPolygon(polygon2);
            painter.drawPolygon(polygon3);
            painter.drawPolygon(polygon4);
            painter.drawPolygon(polygon5);
            painter.drawPolygon(polygon6);
        }
    }

    int** areaMatrix = new int*[yPixels];
    for (int i = 0; i < yPixels; i++)
        areaMatrix[i] = new int[xPixels];

    unsigned char* bitmap = new unsigned char[xPixels*yPixels];

    QRgb black = qRgb(0, 0, 0);

    for (int i = 0; i < xPixels; i++)
    {
        for (int j = 0; j < yPixels; j++)
        {
            int pixelIntensity = 0;
            for (int qi = i - 1; qi <= i + 1; ++qi)
            for (int qj = j - 1; qj <= j + 1; ++qj)
            {
                if (qi < 0 || qi >= xPixels) continue;
                if (qj < 0 || qj >= yPixels) continue;
                pixelIntensity += sourceImage->pixel(qi, qj) == black ? 1 : 0;
            }

            if (pixelIntensity > 0)
            {
                areaMatrix[j][i] = 1;
                bitmap[i*yPixels + j] = 0;
            }
            else
            {
                areaMatrix[j][i] = 0;
                bitmap[i*yPixels + j] = 255;
            }
        }
    }

//    SoTexture2* texture = static_cast< SoTexture2* >(getPart("iconTexture", true) );
//    texture->image.setValue(SbVec2s(heightPixeles, widthPixeles), 1, bitmap);
//    texture->wrapS = SoTexture2::CLAMP;
//    texture->wrapT = SoTexture2::CLAMP;

    delete[] bitmap;

    SetLightSourceArea(yPixels, xPixels, areaMatrix);
}

void SunAperture::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/ )
{
    box.setBounds(
        SbVec3f(m_xMin, m_yMin, 0.),
        SbVec3f(m_xMax, m_yMax, 0.)
    );
}

void SunAperture::generatePrimitives(SoAction* action)
{
    SoPrimitiveVertex pv;
    SoState* state = action->getState();

    // See if we have to use a texture coordinate function,
    // rather than generating explicit texture coordinates.
    SbBool useTexFunc =  (SoTextureCoordinateElement::getType(state) == SoTextureCoordinateElement::FUNCTION);

    const SoTextureCoordinateElement* tce = 0;
    SbVec4f texCoord;
    if (useTexFunc)
        tce = SoTextureCoordinateElement::getInstance(state);
    else
    {
        texCoord[2] = 0.;
        texCoord[3] = 1.;
    }

    SbVec3f normal(0., 0., -1.);
    beginShape(action, TRIANGLE_STRIP);

     //P1
     SbVec3f point( m_xMin, m_yMax, 0.);
     if (useTexFunc)
         texCoord = tce->get(point, normal);
     else
     {
         texCoord[0] = 0.;
         texCoord[1] = 1.;
     }
    pv.setPoint(point);
    pv.setNormal(normal);
    pv.setTextureCoords(texCoord);
    shapeVertex(&pv);

     //P2
    point = SbVec3f( m_xMin, m_yMin, 0.);

     if (useTexFunc )
         texCoord = tce->get(point, normal);
     else
     {
         texCoord[0] = 0.;
         texCoord[1] = 0.;
     }

    pv.setPoint(point);
    pv.setNormal(normal);
    pv.setTextureCoords(texCoord);
    shapeVertex(&pv);

     //P3
    point = SbVec3f( m_xMax, m_yMax, 0.);
     if (useTexFunc )
         texCoord = tce->get(point, normal);
     else
     {
         texCoord[0] = 1.;
         texCoord[1] = 1.;
     }

    pv.setPoint(point);
    pv.setNormal(normal);
    pv.setTextureCoords(texCoord);
    shapeVertex(&pv);

     //P4
    point = SbVec3f( m_xMax, m_yMin, 0.);
     if (useTexFunc )
         texCoord = tce->get(point, normal);
     else
     {
         texCoord[0] = 1.;
         texCoord[1] = 0.;
     }

    pv.setPoint(point);
    pv.setNormal(normal);
    pv.setTextureCoords(texCoord);
    shapeVertex(&pv);

    endShape();
}
