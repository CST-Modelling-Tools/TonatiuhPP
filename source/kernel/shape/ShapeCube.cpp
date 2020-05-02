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

SbVec3d ShapeCube::frontNormal(0.0, 0.0, 1.0);
SbVec3d ShapeCube::rearNormal(0.0, 0.0, -1.0);
SbVec3d ShapeCube::leftNormal(1.0, 0.0, 0.0);
SbVec3d ShapeCube::rightNormal(-1.0, 0.0, 0.0);
SbVec3d ShapeCube::baseNormal(0.0, -1.0, 0.0);
SbVec3d ShapeCube::topNormal(0.0, 1.0, 0.0);

void ShapeCube::initClass()
{
    SO_NODE_INIT_CLASS(ShapeCube, ShapeAbstract, "ShapeAbstract");
}

ShapeCube::ShapeCube()
{
    SO_NODE_CONSTRUCTOR(ShapeCube);

    SO_NODE_ADD_FIELD(m_widthX, (2.));
    SO_NODE_ADD_FIELD(m_widthY, (2.));
    SO_NODE_ADD_FIELD(m_widthZ, (2.));
}

double ShapeCube::getArea() const
{
    double frontArea = m_widthX.getValue()*m_widthY.getValue();
    double leftArea = m_widthY.getValue()*m_widthZ.getValue();
    double baseArea = m_widthX.getValue()*m_widthZ.getValue();
    return 2.*(frontArea + leftArea + baseArea);
}

double ShapeCube::getVolume() const
{
    return m_widthX.getValue()*m_widthY.getValue()*m_widthZ.getValue();
}

BBox ShapeCube::getBox() const
{
    BBox box;

    double wx = m_widthX.getValue()/2.;
    double wy = m_widthY.getValue()/2.;
    double wz = m_widthZ.getValue()/2.;

    box.pMin = Point3D(-wx, -wy, -wz);
    box.pMax = Point3D(wx, wy, wz);

    return box;
}

bool ShapeCube::intersect(const Ray& /*objectRay*/, double* /*tHit*/, DifferentialGeometry* /*dg*/) const
{
    //Yet to be implemented
    return false;
}

bool ShapeCube::intersectP(const Ray& ray) const
{
    double t0 = ray.tMin;
    double t1 = ray.tMax;
    double tmin, tmax, tymin, tymax, tzmin, tzmax;
    double halfWidth = m_widthX.getValue()/2.0;
    double halfHeight = m_widthY.getValue()/2.0;
    double halfDepth = m_widthZ.getValue()/2.0;

    double invDirection = ray.invDirection().x;
    if (invDirection >= 0.)
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
    if (invDirection >= 0.)
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

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    invDirection = ray.invDirection().z;
    if (invDirection >= 0.)
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

void ShapeCube::generatePrimitives(SoAction* action)
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
   double halfWidth = m_widthX.getValue()/2.0;
   double halfHeight = m_widthY.getValue()/2.0;
   double halfDepth = m_widthZ.getValue()/2.0;

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

Point3D ShapeCube::getPoint(double /*u1*/, double /*u2*/ ) const
{
    //Yet to be implemented
    return Point3D( 0, 0, 0 );
}

Vector3D ShapeCube::getNormal(double /*u*/, double /*v*/ ) const
{
    return Vector3D(0, 1, 0);
}

