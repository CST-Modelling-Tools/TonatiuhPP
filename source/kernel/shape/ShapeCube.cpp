#include <QString>

#include <Inventor/SbLinear.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/misc/SoState.h>

#include "libraries/geometry/BBox.h"
#include "libraries/geometry/NormalVector.h"
#include "libraries/geometry/Point3D.h"
#include "ShapeCube.h"
#include "libraries/geometry/Ray.h"

using namespace std;

SO_NODE_SOURCE(ShapeCube)

// Normals to the six sides of the cube
   SbVec3d ShapeCube::frontNormal, ShapeCube::rearNormal;
   SbVec3d ShapeCube::leftNormal, ShapeCube::rightNormal;
   SbVec3d ShapeCube::baseNormal, ShapeCube::topNormal;

void ShapeCube::initClass()
{
    SO_NODE_INIT_CLASS(ShapeCube, ShapeAbstract, "TShape");
}

ShapeCube::ShapeCube()
{
    SO_NODE_CONSTRUCTOR(ShapeCube);
    SO_NODE_ADD_FIELD(m_width, (2.0));
    SO_NODE_ADD_FIELD(m_height, (2.0));
    SO_NODE_ADD_FIELD(m_depth, (2.0));

    // If this is the first time the constructor is called, set
    // up the static normals.
    if ( SO_NODE_IS_FIRST_INSTANCE() )
    {
        frontNormal.setValue( 0.0, 0.0, 1.0 );
        rearNormal.setValue( 0.0, 0.0, -1.0 );
        leftNormal.setValue( 1.0, 0.0, 0.0 );
        rightNormal.setValue( -1.0, 0.0, 0.0 );
        baseNormal.setValue( 0.0, -1.0, 0.0 );
        topNormal.setValue( 0.0, 1.0, 0.0 );
    }
}

ShapeCube::~ShapeCube()
{

}


double ShapeCube::GetArea() const
{
    double frontArea = m_width.getValue() * m_height.getValue();
    double leftArea = m_height.getValue() * m_depth.getValue();
    double baseArea = m_depth.getValue() * m_depth.getValue();

    return ( 2 * frontArea ) + ( 2 * leftArea ) + (2 * baseArea );

}

double ShapeCube::GetVolume() const
{
    return m_width.getValue() * m_height.getValue() * m_depth.getValue();
}

/*!
 * Return the shape bounding box.
 */
BBox ShapeCube::GetBBox() const
{
    BBox bBox;

    // Compute the half-width, half-height, and half-depth of
     // the pyramid. We'll use this info to set the min and max
     // points.
    double halfWidth = m_width.getValue()/2.0;
    double halfHeight = m_height.getValue()/2.0;
    double halfDepth = m_depth.getValue()/2.0;

    bBox.pMin = Point3D( -halfWidth, -halfHeight, -halfDepth );
    bBox.pMax = Point3D( halfWidth, halfHeight, halfDepth );

    return bBox;
}

QString ShapeCube::GetIcon() const
{
    return QLatin1String( ":/icons/tcube.png" );
}

bool ShapeCube::Intersect(const Ray& /*objectRay*/, double* /*tHit*/, DifferentialGeometry* /*dg*/) const
{
    //Yet to be implemented
    return false;
}

bool ShapeCube::IntersectP( const Ray& ray ) const
{
    double t0 = ray.tMin;
    double t1 = ray.tMax;
    double tmin, tmax, tymin, tymax, tzmin, tzmax;
    double halfWidth = m_width.getValue()/2.0;
    double halfHeight = m_height.getValue()/2.0;
    double halfDepth = m_depth.getValue()/2.0;

    double invDirection = ray.invDirection().x;
    if( invDirection >= 0.0 )
    {
       tmin = ( -halfWidth - ray.origin.x ) * invDirection;
       tmax = ( halfWidth - ray.origin.x ) * invDirection;
    }
    else
    {
       tmin = ( halfWidth - ray.origin.x ) * invDirection;
       tmax = ( -halfWidth - ray.origin.x ) * invDirection;
    }

    invDirection = ray.invDirection().y;
    if( invDirection >= 0.0 )
    {
       tymin = ( -halfHeight - ray.origin.y ) * invDirection;
       tymax = ( halfHeight - ray.origin.y ) * invDirection;
    }
    else
    {
       tymin = ( halfHeight - ray.origin.y ) * invDirection;
       tymax = ( -halfHeight - ray.origin.y ) * invDirection;
    }

    if ( ( tmin > tymax ) || ( tymin > tmax ) ) return false;

    if ( tymin > tmin ) tmin = tymin;
    if ( tymax < tmax ) tmax = tymax;

    invDirection = ray.invDirection().z;
    if( invDirection >= 0.0 )
    {
       tzmin = ( -halfDepth - ray.origin.z ) * invDirection;
       tzmax = ( halfDepth - ray.origin.z ) * invDirection;
    }
    else
    {
       tzmin = ( halfDepth - ray.origin.z ) * invDirection;
       tzmax = ( -halfDepth - ray.origin.z ) * invDirection;
    }

    if ( ( tmin > tzmax ) || ( tzmin > tmax ) ) return false;

    if ( tzmin > tmin ) tmin = tzmin;
    if ( tzmax < tmax ) tmax = tzmax;
    if ( ( tmin < t1 ) && ( tmax > t0 ) )
    {
        return true;
    }
    else return false;
}

void ShapeCube::computeBBox(SoAction*, SbBox3f& box, SbVec3f& center)
{
    BBox bBox = GetBBox();
    // These points define the min and max extents of the box.
    SbVec3f min, max;

    min.setValue( bBox.pMin.x, bBox.pMin.y, bBox.pMin.z );
    max.setValue( bBox.pMax.x, bBox.pMax.y, bBox.pMax.z );;

    // Set the box to bound the two extreme points.
    box.setBounds(min, max);

   // This defines the "natural center" of the ShapeCube. We could
   // define it to be the center of the base, if we want, but
   // let's just make it the center of the bounding box.
   center.setValue(0.0, 0.0, 0.0);
}

void ShapeCube::generatePrimitives(SoAction *action)
{
    // The ShapeCube will generate 6 quads: 1 for each side
    // This variable is used to store each vertex.
    SoPrimitiveVertex pv;

    // Access the state from the action.
    SoState  *state = action->getState();

    // See if we have to use a texture coordinate function,
    // rather than generating explicit texture coordinates.
    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

   // If we need to generate texture coordinates with a
   // function, we'll need an SoGLTextureCoordinateElement.
   // Otherwise, we'll set up the coordinates directly.
   const SoTextureCoordinateElement* tce = 0;
   SbVec4f texCoord;
   if ( useTexFunc )    tce = SoTextureCoordinateElement::getInstance( state );
   else {
     texCoord[2] = 0.0;
     texCoord[3] = 1.0;
   }


   // We need the size to adjust the coordinates.
   double halfWidth = m_width.getValue()/2.0;
   double halfHeight = m_height.getValue()/2.0;
   double halfDepth = m_depth.getValue()/2.0;

   // We'll use this macro to make the code easier. It uses the
   // "point" variable to store the primitive vertex's point.
   SbVec3f  point;


#define GEN_VERTEX(pv, x, y, z, s, t, normal)   \
     point.setValue(halfWidth  * x,             \
                halfHeight * y,                 \
                halfDepth  * z);                \
     if (useTexFunc)                            \
       texCoord = tce->get(SbVec3f( point ), SbVec3f( normal ) );      \
     else {                                     \
       texCoord[0] = s;                         \
       texCoord[1] = t;                         \
     }                                          \
     pv.setPoint(point);                        \
     pv.setNormal( SbVec3f( normal ) );         \
     pv.setTextureCoords(texCoord);             \
     shapeVertex(&pv)

     // We will generate two triangles for the base, as a
     // triangle strip.
     beginShape(action, QUADS);

     // Base: left front, left rear, right front, right rear
     GEN_VERTEX(pv,  1.0, -1.0,  1.0, 1.0,  1.0, baseNormal);
     GEN_VERTEX(pv,  1.0, -1.0, -1.0, 0.0,  1.0, baseNormal);
     GEN_VERTEX(pv, -1.0, -1.0, -1.0, 0.0,  0.0, baseNormal);
     GEN_VERTEX(pv, -1.0, -1.0,  1.0, 1.0,  0.0, baseNormal);

     // Top: left front, left rear, right front, right rear
     GEN_VERTEX(pv,  1.0, 1.0,  1.0, 1.0,  1.0, topNormal);
     GEN_VERTEX(pv,  1.0, 1.0, -1.0, 0.0,  1.0, topNormal);
     GEN_VERTEX(pv, -1.0, 1.0, -1.0, 0.0,  0.0, topNormal);
     GEN_VERTEX(pv, -1.0, 1.0,  1.0, 1.0,  0.0, topNormal);

     // Front
     GEN_VERTEX(pv,  1.0, -1.0, 1.0, 1.0,  1.0, frontNormal);
     GEN_VERTEX(pv,  1.0,  1.0, 1.0, 0.0,  1.0, frontNormal);
     GEN_VERTEX(pv, -1.0,  1.0, 1.0, 0.0,  0.0, frontNormal);
     GEN_VERTEX(pv, -1.0, -1.0, 1.0, 1.0,  0.0, frontNormal);

      // Rear
     GEN_VERTEX(pv,  1.0, -1.0, -1.0, 1.0,  1.0, frontNormal);
     GEN_VERTEX(pv,  1.0,  1.0, -1.0, 0.0,  1.0, frontNormal);
     GEN_VERTEX(pv, -1.0,  1.0, -1.0, 0.0,  0.0, frontNormal);
     GEN_VERTEX(pv, -1.0, -1.0, -1.0, 1.0,  0.0, frontNormal);

     //Left
     GEN_VERTEX(pv,  1.0, -1.0,  1.0, 1.0,  1.0, leftNormal);
     GEN_VERTEX(pv,  1.0,  1.0,  1.0, 0.0,     1.0, leftNormal);
     GEN_VERTEX(pv,  1.0,  1.0, -1.0, 0.0,  0.0, leftNormal);
     GEN_VERTEX(pv,  1.0, -1.0, -1.0, 1.0,  0.0, leftNormal);

      //Right
     GEN_VERTEX(pv, -1.0, -1.0,  1.0, 1.0,  1.0, leftNormal);
     GEN_VERTEX(pv, -1.0,  1.0,  1.0, 0.0,  1.0, leftNormal);
     GEN_VERTEX(pv, -1.0,  1.0, -1.0, 0.0,  0.0, leftNormal);
     GEN_VERTEX(pv, -1.0, -1.0, -1.0, 1.0,  0.0, leftNormal);

     endShape();

}

Point3D ShapeCube::Sample( double /*u1*/, double /*u2*/ ) const
{
    //Yet to be implemented
    return Point3D( 0, 0, 0 );
}

Point3D ShapeCube::GetPoint3D( double /*u1*/, double /*u2*/ ) const
{
    //Yet to be implemented
    return Point3D( 0, 0, 0 );
}

NormalVector ShapeCube::GetNormal(double /*u*/, double /*v*/ ) const
{
    return NormalVector( 0, 1, 0 );
}

