#include "TLightShape.h"

#include <QString>
#include <QVector>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>

#include <math.h>
#include "libraries/geometry/BoundingBox.h"
#include "libraries/geometry/Ray.h"
#include "libraries/geometry/Transform.h"
#include "libraries/geometry/Vector3D.h"
#include "libraries/geometry/gf.h"
#include "scene/TShapeKit.h"
#include "shape/DifferentialGeometry.h"


SO_NODE_SOURCE(TLightShape)

void TLightShape::initClass()
{
    SO_NODE_INIT_CLASS(TLightShape, SoShape, "Shape");
}

TLightShape::TLightShape():
    m_heightElements(0),
    m_lightAreaMatrix(0),
    m_widthElements(0)
{
    SO_NODE_CONSTRUCTOR(TLightShape);
    SO_NODE_ADD_FIELD( xMin, (-0.5) );
    SO_NODE_ADD_FIELD( xMax, (0.5) );
    SO_NODE_ADD_FIELD( yMin, (-0.5) );
    SO_NODE_ADD_FIELD( yMax, (0.5) );
    SO_NODE_ADD_FIELD( delta, (100.) );
}

TLightShape::~TLightShape()
{
    for( int i = 0; i < m_heightElements; i++ )
        delete[] m_lightAreaMatrix[i];
    delete[] m_lightAreaMatrix;
}

double TLightShape::GetValidArea() const
{
    int numberOfValidAreas = m_validAreasVector.size();

    double xWidth =  xMax.getValue() - xMin.getValue();
    double pixelWidth = xWidth / m_widthElements;

    double yWidth = yMax.getValue() - yMin.getValue();
    double pixelHeight = yWidth / m_heightElements;

    double validArea = (pixelWidth*pixelHeight) * numberOfValidAreas;

    return validArea;
}

/*!
 * Returns the indexes of the valid areas to the ray tracer.
 */
Point3D TLightShape::GetPoint3D(double u, double v, int h, int w) const //? w <> h
{
    if ( OutOfRange(u, v) )
        gf::SevereError("Function TLightShape::GetPoint3D called with invalid parameters" );

    // size of cells the sun is divided
    double xWidth = (xMax.getValue() - xMin.getValue())/m_widthElements;
    double zWidth = (yMax.getValue() - yMin.getValue())/m_heightElements;

    // calculate the photon coordinate
    double x = xMin.getValue() + (u + w)*xWidth;
    double y = yMin.getValue() + (v + h)*zWidth;

    return Point3D(x, y, 0.);
}

void TLightShape::SetLightSourceArea(int h, int w, int** lightArea)
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

    m_validAreasVector.clear();

    for( int i = 0; i < m_heightElements; i++ )
        for( int j = 0; j < m_widthElements; j++ )
            if( m_lightAreaMatrix[i][j] == 1 )
                m_validAreasVector.push_back( QPair< int, int >( i, j ) );

}

void TLightShape::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/ )
{
    SbVec3f min(xMin.getValue(), yMin.getValue(), 0.);
    SbVec3f max(xMax.getValue(), yMax.getValue(), 0.);
    box.setBounds(min, max);
}

void TLightShape::generatePrimitives(SoAction* action)
{
    SoPrimitiveVertex   pv;
    SoState  *state = action->getState();

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
     SbVec3f point( xMin.getValue(), yMax.getValue(), 0.);
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
    point = SbVec3f( xMin.getValue(), yMin.getValue(), 0.);

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
    point = SbVec3f( xMax.getValue(), yMax.getValue(), 0.);
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
    point = SbVec3f( xMax.getValue(), yMin.getValue(), 0.);
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
