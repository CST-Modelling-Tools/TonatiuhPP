#include <QString>
#include <QVector>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>

#include <math.h>

#include "libraries/geometry/gf.h"

#include "libraries/geometry/BBox.h"
#include "DifferentialGeometry.h"
#include "libraries/geometry/Ray.h"
#include "TLightShape.h"
#include "libraries/geometry/Transform.h"
#include "TShapeKit.h"
#include "libraries/geometry/Vector3D.h"



SO_NODE_SOURCE(TLightShape);

void TLightShape::initClass()
{
    SO_NODE_INIT_CLASS(TLightShape, SoShape, "Shape");
}

TLightShape::TLightShape( )
:m_heightElements( 0 ),
 m_lightAreaMatrix( 0 ),
 m_widthElements( 0 )
{
    SO_NODE_CONSTRUCTOR(TLightShape);
    SO_NODE_ADD_FIELD( xMin, (-0.5) );
    SO_NODE_ADD_FIELD( xMax, (0.5) );
    SO_NODE_ADD_FIELD( zMin, (-0.5) );
    SO_NODE_ADD_FIELD( zMax, (0.5) );
    SO_NODE_ADD_FIELD( delta, (100.0) );
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

    double width =  xMax.getValue() - xMin.getValue();
    double pixelWidth = width / m_widthElements;

    double height = zMax.getValue() - zMin.getValue();
    double pixelHeight = height / m_heightElements;

    double validArea = ( pixelWidth * pixelHeight ) * numberOfValidAreas;

    return validArea;
}


/*!
 * Returns the indexes of the valid areas to the ray tracer.
 */

std::vector< QPair< int, int > > TLightShape::GetValidAreasCoord() const
{

    return m_validAreasVector;
}

Point3D TLightShape::Sample( double u, double v, int a, int b) const
{
    //calculate the coordinates of a photon un a cell
    return GetPoint3D( u, v,a,b );
}

Point3D TLightShape::GetPoint3D( double u, double v, int h, int w ) const
{
    if( OutOfRange( u, v ) )     gf::SevereError("Function TLightShape::GetPoint3D called with invalid parameters" );

    //size of cells the sun is divided
    double width =  (xMax.getValue() - xMin.getValue())/m_widthElements;
    double height = (zMax.getValue() - zMin.getValue())/m_heightElements;

    //calculate the photon coordinate
    double x = xMin.getValue()+( u * width ) + (w*width);
    double z = zMin.getValue()+( v * height ) + (h*height);

    return Point3D( x, 0, z );
}

void TLightShape::SetLightSourceArea( int h, int w, int** lightArea )
{

    if( m_lightAreaMatrix != 0 )
    {
        for( int i = 0; i < m_heightElements; i++ )
            delete[] m_lightAreaMatrix[i];
        delete[] m_lightAreaMatrix;
    }

    m_heightElements = h;
    m_widthElements = w;
    m_lightAreaMatrix = lightArea;

    m_validAreasVector.clear();

    for( int i = 0; i < m_heightElements; i++ )
        for( int j = 0; j < m_widthElements; j++ )
            if( m_lightAreaMatrix[i][j] == 1 )    m_validAreasVector.push_back( QPair< int, int >( i, j ) );

}

bool TLightShape::OutOfRange( double u, double v ) const
{
    return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

void TLightShape::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/ )
{
    // These points define the min and max extents of the box.
    SbVec3f min, max;

    min.setValue( xMin.getValue(), 0.0, zMin.getValue() );
    max.setValue( xMax.getValue(), 0.0, zMax.getValue() );

    // Set the box to bound the two extreme points.
    box.setBounds( min, max );

}

void TLightShape::generatePrimitives(SoAction *action)
{
    SoPrimitiveVertex   pv;
    SoState  *state = action->getState();

    // See if we have to use a texture coordinate function,
    // rather than generating explicit texture coordinates.
    SbBool useTexFunc =  (SoTextureCoordinateElement::getType(state) == SoTextureCoordinateElement::FUNCTION);

    const SoTextureCoordinateElement* tce = 0;
    SbVec4f texCoord;
    if (useTexFunc)    tce = SoTextureCoordinateElement::getInstance(state);
    else
    {
        texCoord[2] = 0.0;
        texCoord[3] = 1.0;
    }

    SbVec3f normal( 0.0, -1.0, 0.0 );
    beginShape(action, TRIANGLE_STRIP);

     //P1
     SbVec3f point( xMin.getValue(), 0.0, zMax.getValue() );
     if (useTexFunc )
         texCoord = tce->get(point, normal);
     else
     {
         texCoord[0] = 1.0;
         texCoord[1] = 0.0;
     }

    pv.setPoint( point );
    pv.setNormal( normal );
    pv.setTextureCoords( texCoord );
    shapeVertex( &pv );

     //P2
    point = SbVec3f( xMin.getValue(), 0.0, zMin.getValue() );

     if (useTexFunc )
         texCoord = tce->get(point, normal);
     else
     {
         texCoord[0] = 0.0;
         texCoord[1] = 0.0;
     }

    pv.setPoint( point );
    pv.setNormal( normal );
    pv.setTextureCoords( texCoord );
    shapeVertex( &pv );

     //P3
    point = SbVec3f( xMax.getValue(), 0.0, zMax.getValue() );
     if (useTexFunc )
         texCoord = tce->get(point, normal);
     else
     {
         texCoord[0] = 1.0;
         texCoord[1] = 1.0;
     }

    pv.setPoint( point );
    pv.setNormal( normal );
    pv.setTextureCoords( texCoord );
    shapeVertex( &pv );

     //P4
    point = SbVec3f( xMax.getValue(), 0.0, zMin.getValue() );
     if (useTexFunc )
         texCoord = tce->get(point, normal);
     else
     {
         texCoord[0] = 0.0;
         texCoord[1] = 1.0;
     }

    pv.setPoint( point );
    pv.setNormal( SbVec3f( 0.0, -1.0, 0.0 ) );
    pv.setTextureCoords( texCoord );
    shapeVertex( &pv );

     endShape();

}
