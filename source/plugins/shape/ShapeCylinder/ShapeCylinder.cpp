#include <QString>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include "libraries/geometry/gc.h"
#include "libraries/geometry/gf.h"
#include "libraries/geometry/BBox.h"
#include "libraries/geometry/Ray.h"
#include "libraries/geometry/Vector3D.h"

#include "kernel/raytracing/DifferentialGeometry.h"
#include "ShapeCylinder.h"

SO_NODE_SOURCE(ShapeCylinder);

void ShapeCylinder::initClass()
{
	SO_NODE_INIT_CLASS(ShapeCylinder, TShape, "TShape");
}

ShapeCylinder::ShapeCylinder( )
{
	SO_NODE_CONSTRUCTOR(ShapeCylinder);
	SO_NODE_ADD_FIELD( radius, (0.5) );
	SO_NODE_ADD_FIELD( length, (1.0) );
	SO_NODE_ADD_FIELD( phiMax, (gc::TwoPi) );

	SO_NODE_DEFINE_ENUM_VALUE( Side, INSIDE );
	SO_NODE_DEFINE_ENUM_VALUE( Side, OUTSIDE );
	SO_NODE_SET_SF_ENUM_TYPE( activeSide, Side );
	SO_NODE_ADD_FIELD( activeSide, (OUTSIDE) );
}

ShapeCylinder::~ShapeCylinder()
{
}

double ShapeCylinder::GetArea() const
{
	return (2 * gc::Pi * radius.getValue() * length.getValue() );
}

double ShapeCylinder::GetVolume() const
{
	return (gc::Pi * (radius.getValue()*radius.getValue()) * length.getValue() );
}

/*!
 * Return the shape bounding box.
 */
BBox ShapeCylinder::GetBBox() const
{
	double cosPhiMax = cos( phiMax.getValue() );
	double sinPhiMax = sin( phiMax.getValue() );

	double xmin = ( phiMax.getValue() >= gc::Pi ) ? -radius.getValue() : radius.getValue() * cosPhiMax;
	double xmax = radius.getValue();
	double ymin = 0.0;
	if( phiMax.getValue() > gc::Pi )
		ymin = ( phiMax.getValue() < ( 1.5 * gc::Pi ) ) ? radius.getValue() * sinPhiMax : -radius.getValue();
	double ymax = ( phiMax.getValue() < ( gc::Pi / 2.0 ) )? radius.getValue() * sinPhiMax : radius.getValue();

	double zmin = 0.0;
	double zmax = length.getValue();

	return BBox( Point3D( xmin, ymin, zmin ), Point3D( xmax, ymax, zmax ) );
}

bool ShapeCylinder::Intersect( const Ray& ray, double* tHit, DifferentialGeometry* dg ) const
{
	// Compute quadratic cylinder coefficients
//	Vector3D vObjectRayOrigin = Vector3D( objectRay.origin );
    double A = ray.direction().x*ray.direction().x + ray.direction().y*ray.direction().y;
    double B = 2.0 * ( ray.direction().x* ray.origin.x + ray.direction().y * ray.origin.y);
    double C = ray.origin.x * ray.origin.x + ray.origin.y * ray.origin.y - radius.getValue() * radius.getValue();

	// Solve quadratic equation for _t_ values
	double t0, t1;
	if( !gf::Quadratic( A, B, C, &t0, &t1 ) ) return false;

	// Compute intersection distance along ray
    if( t0 > ray.tMax || t1 < ray.tMin ) return false;
    double thit = ( t0 > ray.tMin )? t0 : t1 ;
    if( thit > ray.tMax ) return false;

   //Compute possible cylinder hit position and $\phi
    Point3D hitPoint = ray( thit );
	double phi = atan2( hitPoint.y, hitPoint.x );
	if ( phi < 0. ) phi += gc::TwoPi;

	//Evaluate Tolerance
	double tol = 0.00001;
	double zmin = 0.0;
	double zmax = length.getValue();


	// Test intersection against clipping parameters
    if( (thit - ray.tMin) < tol  || hitPoint.z < zmin || hitPoint.z > zmax || phi > phiMax.getValue() )
	{
		if ( thit == t1 ) return false;
        if ( t1 > ray.tMax ) return false;
		thit = t1;

        hitPoint = ray( thit );
		phi = atan2( hitPoint.y, hitPoint.x );
		if ( phi < 0. ) phi += gc::TwoPi;
        if ( (thit - ray.tMin) < tol  || hitPoint.z < zmin || hitPoint.z > zmax || phi > phiMax.getValue() ) return false;
	}
	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) )
		gf::SevereError( "Function Cylinder::Intersect(...) called with null pointers" );


	// Find parametric representation of Cylinder hit
	double u = phi / phiMax.getValue();
	double v = hitPoint.z /length.getValue();

	// Compute cylinder \dpdu and \dpdv
	//double zradius = sqrt( hitPoint.x*hitPoint.x + hitPoint.y*hitPoint.y );
	//double invzradius = 1.0 / zradius;

	Vector3D dpdu( -phiMax.getValue() * radius.getValue() * sin ( phiMax.getValue() * u ),
						phiMax.getValue() * radius.getValue() * cos( phiMax.getValue() * u ),
						0.0 );
	Vector3D dpdv( 0.0, 0.0, length.getValue() );


    NormalVector N = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );


    *dg = DifferentialGeometry(hitPoint, u, v, dpdu, dpdv, N, this);
    dg->shapeFrontSide = DotProduct(N, ray.direction()) <= 0.;

    *tHit = thit;

	return true;
}

bool ShapeCylinder::IntersectP(const Ray& worldRay) const
{
    return Intersect(worldRay, 0, 0);
}


Point3D ShapeCylinder::GetPoint3D (double u, double v) const
{
	if ( OutOfRange( u, v ) )
		gf::SevereError( "Function Function Poligon::GetPoint3D called with invalid parameters" );

	double phi = u * phiMax.getValue();
	double ilength = v * length.getValue();

	double x = radius.getValue()*cos (phi);
	double y = radius.getValue()*sin (phi);
	double z = ilength;

	return Point3D (x, y, z);
}

NormalVector ShapeCylinder::GetNormal (double u, double /* v */) const
{
	Vector3D dpdu( -phiMax.getValue() * radius.getValue() * sin ( phiMax.getValue() * u ),
					phiMax.getValue() * radius.getValue() * cos( phiMax.getValue() * u ),
					0.0 );
	Vector3D dpdv( 0.0, 0.0, length.getValue() );

	return Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );

}

void ShapeCylinder::computeBBox(SoAction *, SbBox3f &box, SbVec3f& /*center*/ )
{
	BBox bBox = GetBBox();
	// These points define the min and max extents of the box.
    SbVec3f min, max;

    min.setValue( bBox.pMin.x, bBox.pMin.y, bBox.pMin.z );
    max.setValue( bBox.pMax.x, bBox.pMax.y, bBox.pMax.z );;

    // Set the box to bound the two extreme points.
    box.setBounds(min, max);
}

void ShapeCylinder::generatePrimitives(SoAction *action)
{
	SoPrimitiveVertex   pv;

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

	if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);


	const int rows = 25; // Number of points per row
	const int columns = 5; // Number of points per column
	const int totalPoints = (rows)*(columns); // Total points in the grid

	float vertex[totalPoints][6];

	int h = 0;
	double ui = 0;
	double vj = 0;

	for (int i = 0; i < rows; ++i )
	{
		ui =( 1.0 /(double)(rows-1) ) * i;

		for ( int j = 0 ; j < columns ; ++j )
		{

			vj = ( 1.0 /(double)(columns-1) ) * j;

			Point3D point = GetPoint3D(ui, vj);
			NormalVector normal;
			if( activeSide.getValue() == 0 )	normal = -GetNormal(ui, vj);
			else	normal = GetNormal(ui, vj);

			vertex[h][0] = point.x;
			vertex[h][1] = point.y;
			vertex[h][2] = point.z;
			vertex[h][3] = normal.x;
			vertex[h][4] = normal.y;
			vertex[h][5] = normal.z;


			pv.setPoint( vertex[h][0], vertex[h][1], vertex[h][2] );
			h++; //Increase h to the next point.

		}
	}


	//const int totalIndices  = (rows-1)*(columns-1)*4;

    float u = 1;
    float v = 1;

	beginShape(action, QUADS );
	for( int irow = 0; irow < (rows-1); ++irow )
	{
		for( int icolumn = 0; icolumn < (columns-1); ++icolumn )
		{
			//indices[k] = irow*columns + icolumn;
			int index0 = irow*columns + icolumn;
			SbVec3f  point0( vertex[index0][0], vertex[index0][1],  vertex[index0][2] );
			SbVec3f normal0(vertex[index0][3], vertex[index0][4], vertex[index0][5] );
			SbVec4f texCoord0 = useTexFunc ? tce->get(point0, normal0): SbVec4f( u,v, 0.0, 1.0 );
			pv.setPoint(point0);
			pv.setNormal(normal0);
			pv.setTextureCoords(texCoord0);
			shapeVertex(&pv);

			int index1 = index0 + 1;
			SbVec3f  point1( vertex[index1][0], vertex[index1][1],  vertex[index1][2] );
			SbVec3f normal1(vertex[index1][3], vertex[index1][4], vertex[index1][5] );
			SbVec4f texCoord1 = useTexFunc ? tce->get(point1, normal1): SbVec4f( u,v, 0.0, 1.0 );
			pv.setPoint(point1);
			pv.setNormal(normal1);
			pv.setTextureCoords(texCoord1);
			shapeVertex(&pv);


			int index3 = index0 + columns;
			int index2 = index3 + 1;


			SbVec3f  point2( vertex[index2][0], vertex[index2][1],  vertex[index2][2] );
			SbVec3f normal2(vertex[index2][3], vertex[index2][4], vertex[index2][5] );
			SbVec4f texCoord2 = useTexFunc ? tce->get(point2, normal2): SbVec4f( u,v, 0.0, 1.0 );
			pv.setPoint(point2);
			pv.setNormal(normal2);
			pv.setTextureCoords(texCoord2);
			shapeVertex(&pv);

			SbVec3f  point3( vertex[index3][0], vertex[index3][1],  vertex[index3][2] );
			SbVec3f normal3(vertex[index3][3], vertex[index3][4], vertex[index3][5] );
			SbVec4f texCoord3 = useTexFunc ? tce->get(point3, normal3): SbVec4f( u,v, 0.0, 1.0 );
			pv.setPoint(point3);
			pv.setNormal(normal3);
			pv.setTextureCoords(texCoord3);
			shapeVertex(&pv);

		}
	}

	endShape();

}
