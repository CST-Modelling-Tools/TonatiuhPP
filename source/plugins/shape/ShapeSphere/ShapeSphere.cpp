#include <iostream>

#include <QMessageBox>
#include <QObject>
#include <QString>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "libraries/geometry/gc.h"
#include "libraries/geometry/gf.h"
#include "libraries/geometry/BBox.h"
#include "libraries/geometry/Ray.h"

#include "kernel/raytracing/DifferentialGeometry.h"
#include "ShapeSphere.h"

SO_NODE_SOURCE(ShapeSphere);



void ShapeSphere::initClass()
{
	SO_NODE_INIT_CLASS(ShapeSphere, TShape, "TShape");

}

ShapeSphere::ShapeSphere( )
:m_lastValidRadius( 0.5 ),
 m_lastValidYMax( 0.5 ),
 m_lastValidYMin( -0.5 ),
 m_radiusSensor( 0 ),
 m_yMinSensor( 0 ),
 m_yMaxSensor( 0 ),
 m_phiMaxSensor( 0 )
{
	SO_NODE_CONSTRUCTOR(ShapeSphere);
	SO_NODE_ADD_FIELD( radius, (0.5) );
	SO_NODE_ADD_FIELD( yMin, (-0.5) );
	SO_NODE_ADD_FIELD( yMax, (0.5) );
	SO_NODE_ADD_FIELD( phiMax, ( gc::TwoPi) );

	SO_NODE_DEFINE_ENUM_VALUE( Side, INSIDE );
	SO_NODE_DEFINE_ENUM_VALUE( Side, OUTSIDE );
	SO_NODE_SET_SF_ENUM_TYPE( activeSide, Side );
	SO_NODE_ADD_FIELD( activeSide, (OUTSIDE) );

	SoFieldSensor* m_radiusSensor = new SoFieldSensor(updateRadius, this);
	m_radiusSensor->setPriority( 1 );
	m_radiusSensor->attach( &radius );
	SoFieldSensor* m_yMinSensor = new SoFieldSensor(updateYMin, this);
	m_yMinSensor->setPriority( 1 );
	m_yMinSensor->attach( &yMin );
	SoFieldSensor* m_yMaxSensor = new SoFieldSensor(updateYMax, this);
	m_yMaxSensor->setPriority( 1 );
	m_yMaxSensor->attach( &yMax );
	SoFieldSensor* m_phiMaxSensor = new SoFieldSensor(updatePhiMax, this);
	m_phiMaxSensor->setPriority( 1 );
	m_phiMaxSensor->attach( &phiMax );
}

ShapeSphere::~ShapeSphere()
{
	delete m_radiusSensor;
	delete m_yMinSensor;
	delete m_yMaxSensor;
	delete m_phiMaxSensor;
}

SoNode* ShapeSphere::copy( SbBool copyConnections ) const
{
	// Use the standard version of the copy method to create
	// a copy of this instance, including its field data
	ShapeSphere* newShapeSphere = dynamic_cast< ShapeSphere* >( SoNode::copy( copyConnections ) );

	// Copy the m_thetaMin, m_thetaMax private members explicitly
	newShapeSphere->m_lastValidYMax = m_lastValidYMax;
	newShapeSphere->m_lastValidYMin = m_lastValidYMin;

	return newShapeSphere;
}

double ShapeSphere::GetArea() const
{
	return ( 4 * gc::Pi * radius.getValue() * radius.getValue() );
}
double ShapeSphere::GetVolume() const
{
	return ( 4 * gc::Pi * radius.getValue() * radius.getValue() * radius.getValue() /3 );
}

BBox ShapeSphere::GetBBox() const
{
	double cosPhiMax = cos( phiMax.getValue() );
   	double sinPhiMax = sin( phiMax.getValue() );

   	double thetaMin = acos( yMax.getValue() / radius.getValue() );
   	double thetaMax = acos( yMin.getValue()/radius.getValue() );
   	double maxRadius = ( yMax.getValue() * yMin.getValue() > 0.0 ) ?  std::max( sin(thetaMin) * radius.getValue() , sin(thetaMax) * radius.getValue() )
																	: radius.getValue();
   	double minRadius = std::min( sin(thetaMin) * radius.getValue(), sin(thetaMax) * radius.getValue() );
   	double xmin = ( phiMax.getValue() < gc::Pi  ) ?  0.0
								: ( phiMax.getValue() < 1.5 * gc::Pi  ) ? sinPhiMax * maxRadius
										: -maxRadius;
   	double xmax = ( phiMax.getValue() >= gc::Pi / 2 ) ? maxRadius : sinPhiMax * maxRadius;

   	double ymin = yMin.getValue();
	double ymax = yMax.getValue();

	double zmin = ( phiMax.getValue() > gc::Pi ) ? -maxRadius
					:( phiMax.getValue() > gc::Pi / 2 ) ? maxRadius* cosPhiMax : std::min( maxRadius * cosPhiMax, minRadius * cosPhiMax );
	double zmax = maxRadius;

	return BBox( Point3D( xmin, ymin, zmin), Point3D( xmax, ymax, zmax) );
}

Point3D ShapeSphere::Sample(double u1, double u2) const
{
    return GetPoint3D(u1, u2);
}

bool ShapeSphere::Intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg ) const
{
    // intersection with full shape
    // |r0 + t*d|^2 = R^2  (local coordinates)
    // a*t^2 + b*t + c = 0
    Vector3D rayOrigin = Vector3D(ray.origin);
    double r = radius.getValue();
    double A = ray.direction().lengthSquared();
    double B = 2. * DotProduct( rayOrigin, ray.direction() );
    double C = rayOrigin.lengthSquared() - r*r;
	double t0, t1;
    if (!gf::Quadratic(A, B, C, &t0, &t1)) return false;

    // ray
    if (t0 > ray.tMax || t1 < ray.tMin) return false;
    double thit = ( t0 > ray.tMin )? t0 : t1 ;
    if( thit > ray.tMax ) return false;

    // intersection with clipped shape
    double tolerance = 1e-5;

	// Compute ShapeSphere hit position and $\phi$
    Point3D hitPoint = ray(thit);
    double phi = atan2(hitPoint.x, hitPoint.z);
    if (phi < 0.) phi += gc::TwoPi;

	// Test intersection against clipping parameters
    if( (thit - ray.tMin) < tolerance || hitPoint.y < yMin.getValue() || hitPoint.y > yMax.getValue() || phi > phiMax.getValue() )
	{
		if ( thit == t1 ) return false;
        if ( t1 > ray.tMax ) return false;
		thit = t1;

		// Compute ShapeSphere hit position and $\phi$
        hitPoint = ray(thit);
        phi = atan2(hitPoint.x, hitPoint.z);
        if (phi < 0.) phi += gc::TwoPi;

        if ( (thit - ray.tMin) < tolerance || hitPoint.y < yMin.getValue() || hitPoint.y > yMax.getValue() || phi > phiMax.getValue() )	return false;
	}

    // differential geometry
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) gf::SevereError( "Function ShapeSphere::Intersect(...) called with null pointers" );

	// Find parametric representation of ShapeSphere hit
    double cosTheta = hitPoint.y / radius.getValue();
    double theta = acos(cosTheta);
    double thetaMin = acos(yMax.getValue() / radius.getValue() );
    double thetaMax = acos(yMin.getValue() / radius.getValue() );
    double u = (theta - thetaMin) / (thetaMax - thetaMin);
    double v = phi / phiMax.getValue();

    // Compute ShapeSphere \dpdu and \dpdv
    double sinTheta = sin(theta);
    double cosPhi = cos(phi);
    double sinPhi = sin(phi);

    Vector3D dpdu(
        cosTheta*sinPhi,
        -sinTheta,
        cosTheta*cosPhi
        );
    dpdu *= r*(thetaMax - thetaMin);

    Vector3D dpdv(
        cosPhi,
        0.,
        -sinPhi
        );
    dpdv *= r*phiMax.getValue()*sinTheta;

    Vector3D N(
        sinTheta*sinPhi,
        cosTheta,
        sinTheta*cosPhi
        );

//	Vector3D N = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );

    Vector3D dndu;
    Vector3D dndv;

	// Initialize _DifferentialGeometry_ from parametric information
	*dg = DifferentialGeometry( hitPoint ,
		                        dpdu,
								dpdv,
                                dndu,
								dndv,
                                u, v, this);

    dg->shapeFrontSide = ( DotProduct( N, ray.direction() ) <= 0 );
    // Update _tHit_ for quadric intersection
    *tHit = thit;

	return true;
}

bool ShapeSphere::IntersectP(const Ray& ray) const
{
    return Intersect(ray, 0, 0);
}

void ShapeSphere::updateYMin( void *data, SoSensor * )
{
	ShapeSphere* shapeSphere = (ShapeSphere *) data;
	if( shapeSphere->yMin.getValue() >= shapeSphere->yMax.getValue() )
	{
		QMessageBox::warning( 0,
				QLatin1String( "Tonatiuh" ),
				QObject::tr( "Sphere y min must be smaller than y max value. ") );
		shapeSphere->yMin.setValue( shapeSphere->m_lastValidYMin );
	}
	else if( shapeSphere->yMin.getValue() < -shapeSphere->radius.getValue() )
	{
		shapeSphere->yMin.setValue( -shapeSphere->radius.getValue() );
		shapeSphere->m_lastValidYMin = -shapeSphere->radius.getValue();
	}
	else
		shapeSphere->m_lastValidYMin = shapeSphere->yMin.getValue();
}

void ShapeSphere::updateRadius( void *data, SoSensor* )
{

	ShapeSphere* shapeSphere = (ShapeSphere *) data;
	if( ( shapeSphere->radius.getValue() <= 0.0 ) )
	{
		QMessageBox::warning( 0,
				QLatin1String( "Tonatiuh" ),
				QObject::tr( "The sphere radius must be a positive value." ) );
		shapeSphere->radius.setValue( shapeSphere->m_lastValidRadius );

	}
	else if( shapeSphere->radius.getValue() < std::max( std::fabs( shapeSphere->yMin.getValue() ), std::fabs( shapeSphere->yMin.getValue() ) ) )
	{
		QMessageBox::warning( 0, QLatin1String( "Tonatiuh" ),
				QObject::tr( "The sphere radius must equal or greater than y value." ) );
		shapeSphere->radius.setValue( shapeSphere->m_lastValidRadius );
	}
	else
		shapeSphere->m_lastValidRadius = shapeSphere->radius.getValue();

}


void ShapeSphere::updateYMax( void *data, SoSensor* )
{
	ShapeSphere* shapeSphere = (ShapeSphere *) data;
	if( shapeSphere->yMax.getValue() < shapeSphere->yMin.getValue() )
	{
		QMessageBox::warning( 0,
				QLatin1String( "Tonatiuh" ),
				QObject::tr( "Sphere y max must be larger than y min value. ") );
		shapeSphere->yMax.setValue( shapeSphere->m_lastValidYMax );
	}
	else if( shapeSphere->yMax.getValue() > shapeSphere->radius.getValue() )
	{
		shapeSphere->yMax.setValue( shapeSphere->radius.getValue() );
		shapeSphere->m_lastValidYMax = shapeSphere->radius.getValue();
	}
	else
		shapeSphere->m_lastValidYMax = shapeSphere->yMax.getValue();

}

void ShapeSphere::updatePhiMax( void *data, SoSensor* )
{
	ShapeSphere* shapeSphere = (ShapeSphere *) data;
	if( shapeSphere->phiMax.getValue() > gc::TwoPi )	shapeSphere->phiMax.setValue( gc::TwoPi );
	else if ( shapeSphere->phiMax.getValue() < 0.0 )	shapeSphere->phiMax.setValue( 0.0 );

}

Point3D ShapeSphere::GetPoint3D( double u, double v ) const
{
	if ( OutOfRange( u, v ) ) gf::SevereError( "Function Poligon::GetPoint3D called with invalid parameters" );


	double thetaMin = acos( yMax.getValue() / radius.getValue() );
	double thetaMax = acos( yMin.getValue()/radius.getValue() );
	double theta = u *( thetaMax - thetaMin ) + thetaMin;
	double phi = v * phiMax.getValue();

	double x = radius.getValue() * sin( theta ) * sin( phi );
	double y = radius.getValue() * cos( theta );
	double z = radius.getValue() * sin( theta ) * cos( phi );

	return Point3D (x, y, z);

}

NormalVector ShapeSphere::GetNormal(double u, double v ) const
{
	double thetaMin = acos( yMax.getValue() / radius.getValue() );
	double thetaMax = acos( yMin.getValue()/radius.getValue() );

	Vector3D dpdu( radius.getValue() * ( -thetaMin + thetaMax ) * cos( ( -1 + u ) * thetaMin - u * thetaMax ) * sin( phiMax.getValue() * v ),
					radius.getValue() * ( -thetaMin + thetaMax ) * sin( ( -1 + u ) * thetaMin - u * thetaMax ),
					radius.getValue() * ( -thetaMin + thetaMax ) * cos( phiMax.getValue() * v ) * cos( ( -1 + u ) * thetaMin - u * thetaMax ) );

	Vector3D dpdv( -phiMax.getValue() * radius.getValue() * cos( phiMax.getValue() * v ) * sin( ( -1 + u ) * thetaMin - u * thetaMax ),
					0.0,
					phiMax.getValue() * radius.getValue() * sin( phiMax.getValue() * v ) * sin( ( -1 + u ) * thetaMin - u * thetaMax ) );
	NormalVector normal = Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );
	return normal;
}

bool ShapeSphere::OutOfRange( double u, double v ) const
{
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

void ShapeSphere::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/)
{
	BBox bBox = GetBBox();
	// These points define the min and max extents of the box.
	SbVec3f min, max;

	min.setValue( bBox.pMin.x, bBox.pMin.y, bBox.pMin.z );
	max.setValue( bBox.pMax.x, bBox.pMax.y, bBox.pMax.z );

	// Set the box to bound the two extreme points.
	box.setBounds(min, max);
}

void ShapeSphere::generatePrimitives(SoAction *action)
{
	SoPrimitiveVertex   pv;
	SoState  *state = action->getState();

	SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
						  SoTextureCoordinateElement::FUNCTION );

	const SoTextureCoordinateElement* tce = 0;
	if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);


	SbVec3f  point;
	int rows = 50; // Number of points per row
	int columns = 50; // Number of points per column
	int totalPoints = rows * columns; // Total points in the grid

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

	const int totalIndices  = (rows-1) * (columns-1) * 4;
	int32_t* indices = new int32_t[totalIndices];
	int k = 0;
	for( int irow = 0; irow < (rows-1); ++irow )
		for( int icolumn = 0; icolumn < (columns-1); ++icolumn )
		{

			indices[k] = irow * columns + icolumn;
			indices[k+1] = irow * columns + icolumn + 1;
			indices[k+2] = irow * columns + icolumn + columns + 1;
			indices[k+3] = irow * columns + icolumn + columns;

			k+=4; //Set k to the first point of the next face.
		}


	float finalvertex[totalIndices][6];
	for(int ivert = 0; ivert<totalIndices; ++ivert )
	{
		finalvertex[ivert][0] = vertex[indices[ivert]][0];
		finalvertex[ivert][1] = vertex[indices[ivert]][1];
		finalvertex[ivert][2] = vertex[indices[ivert]][2];
		finalvertex[ivert][3] = vertex[indices[ivert]][3];
		finalvertex[ivert][4] = vertex[indices[ivert]][4];
		finalvertex[ivert][5] = vertex[indices[ivert]][5];
	}
	delete[] indices;

	float u = 1;
	float v = 1;

	beginShape(action, QUADS );
	for( int i = 0; i < totalIndices; ++i )
	{
		SbVec3f  point( finalvertex[i][0], finalvertex[i][1],  finalvertex[i][2] );
		SbVec3f normal(finalvertex[i][3],finalvertex[i][4], finalvertex[i][5] );
		SbVec4f texCoord = useTexFunc ? tce->get(point, normal): SbVec4f( u,v, 0.0, 1.0 );

		pv.setPoint(point);
		pv.setNormal(normal);
		pv.setTextureCoords(texCoord);
		shapeVertex(&pv);
	}
	endShape();
}
