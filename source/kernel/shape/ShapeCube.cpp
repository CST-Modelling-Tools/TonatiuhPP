#include "ShapeCube.h"

#include <QString>

#include <Inventor/SbLinear.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/misc/SoState.h>

#include "libraries/geometry/BoundingBox.h"
#include "libraries/geometry/Point3D.h"
#include "libraries/geometry/Ray.h"

using namespace std;

SO_NODE_SOURCE(ShapeCube)



void ShapeCube::initClass()
{
    SO_NODE_INIT_CLASS(ShapeCube, ShapeAbstract, "ShapeAbstract");
}

ShapeCube::ShapeCube()
{
    SO_NODE_CONSTRUCTOR(ShapeCube);

    SO_NODE_ADD_FIELD(sizeX, (2.));
    SO_NODE_ADD_FIELD(sizeY, (2.));
    SO_NODE_ADD_FIELD(sizeZ, (2.));
}

double ShapeCube::getArea() const
{
    double xy = sizeX.getValue()*sizeY.getValue();
    double yz = sizeY.getValue()*sizeZ.getValue();
    double xz = sizeX.getValue()*sizeZ.getValue();
    return 2.*(xy + yz + xz);
}

double ShapeCube::getVolume() const
{
    return sizeX.getValue()*sizeY.getValue()*sizeZ.getValue();
}

BoundingBox ShapeCube::getBox() const
{
    Vector3D p(
        sizeX.getValue(),
        sizeY.getValue(),
        sizeZ.getValue()
    );
    p /= 2.;

    return BoundingBox(-p, p);
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
    double halfWidth = sizeX.getValue()/2.;
    double halfHeight = sizeY.getValue()/2.;
    double halfDepth = sizeZ.getValue()/2.;

    double invDirection = ray.invDirection().x;
    if (invDirection >= 0.)
    {
        tmin = (-halfWidth - ray.origin.x) * invDirection;
        tmax = (halfWidth - ray.origin.x) * invDirection;
    }
    else
    {
        tmin = (halfWidth - ray.origin.x) * invDirection;
        tmax = (-halfWidth - ray.origin.x) * invDirection;
    }

    invDirection = ray.invDirection().y;
    if (invDirection >= 0.)
    {
        tymin = (-halfHeight - ray.origin.y) * invDirection;
        tymax = (halfHeight - ray.origin.y) * invDirection;
    }
    else
    {
        tymin = (halfHeight - ray.origin.y) * invDirection;
        tymax = (-halfHeight - ray.origin.y) * invDirection;
    }

    if (tmin > tymax || tymin > tmax) return false;

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    invDirection = ray.invDirection().z;
    if (invDirection >= 0.)
    {
        tzmin = (-halfDepth - ray.origin.z) * invDirection;
        tzmax = (halfDepth - ray.origin.z) * invDirection;
    }
    else
    {
        tzmin = (halfDepth - ray.origin.z) * invDirection;
        tzmax = (-halfDepth - ray.origin.z) * invDirection;
    }

    if (tmin > tzmax || tzmin > tmax) return false;

    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;
    if (tmin < t1 && tmax > t0)
        return true;
    else
        return false;
}

void ShapeCube::generatePrimitives(SoAction* action)
{
    SbVec3d frontNormal(0.0, 0.0, 1.0);
    SbVec3d rearNormal(0.0, 0.0, -1.0);
    SbVec3d leftNormal(1.0, 0.0, 0.0);
    SbVec3d rightNormal(-1.0, 0.0, 0.0);
    SbVec3d baseNormal(0.0, -1.0, 0.0);
    SbVec3d topNormal(0., 1., 0.);

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
   double halfWidth = sizeX.getValue()/2.0;
   double halfHeight = sizeY.getValue()/2.0;
   double halfDepth = sizeZ.getValue()/2.0;

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

Vector3D ShapeCube::getPoint(double /*u*/, double /*v*/ ) const
{
    //Yet to be implemented
    return Vector3D(0, 0, 0);
}

Vector3D ShapeCube::getNormal(double /*u*/, double /*v*/ ) const
{
    return Vector3D(0, 0, 1);
}

