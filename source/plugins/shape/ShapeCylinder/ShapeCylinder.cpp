#include <QString>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include "libraries/geometry/gc.h"
#include "libraries/geometry/gf.h"
#include "libraries/geometry/BBox.h"
#include "libraries/geometry/Ray.h"
#include "libraries/geometry/Vector3D.h"

#include "kernel/shape/DifferentialGeometry.h"
#include "ShapeCylinder.h"

SO_NODE_SOURCE(ShapeCylinder)


void ShapeCylinder::initClass()
{
    SO_NODE_INIT_CLASS(ShapeCylinder, ShapeAbstract, "ShapeAbstract");
}

ShapeCylinder::ShapeCylinder( )
{
	SO_NODE_CONSTRUCTOR(ShapeCylinder);
	SO_NODE_ADD_FIELD( radius, (0.5) );
    SO_NODE_ADD_FIELD( length, (1.) );
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
    return 2.*gc::Pi*radius.getValue()*length.getValue();
}

double ShapeCylinder::GetVolume() const
{
    return gc::Pi*radius.getValue()*radius.getValue()*length.getValue();
}

/*!
 * Return the shape bounding box.
 */
BBox ShapeCylinder::GetBBox() const
{
    double cosPhiMax = cos(phiMax.getValue());
    double sinPhiMax = sin(phiMax.getValue());

	double xmin = ( phiMax.getValue() >= gc::Pi ) ? -radius.getValue() : radius.getValue() * cosPhiMax;
	double xmax = radius.getValue();
	double ymin = 0.0;
	if( phiMax.getValue() > gc::Pi )
		ymin = ( phiMax.getValue() < ( 1.5 * gc::Pi ) ) ? radius.getValue() * sinPhiMax : -radius.getValue();
	double ymax = ( phiMax.getValue() < ( gc::Pi / 2.0 ) )? radius.getValue() * sinPhiMax : radius.getValue();

    double zmin = 0.;
	double zmax = length.getValue();

    return BBox(
        Point3D(xmin, ymin, zmin),
        Point3D(xmax, ymax, zmax)
    );
}

bool ShapeCylinder::Intersect( const Ray& ray, double* tHit, DifferentialGeometry* dg ) const
{
	// Compute quadratic cylinder coefficients
//	Vector3D vObjectRayOrigin = Vector3D( objectRay.origin );
    double A = ray.direction().x*ray.direction().x + ray.direction().y*ray.direction().y;
    double B = 2.*( ray.direction().x* ray.origin.x + ray.direction().y * ray.origin.y);
    double C = ray.origin.x * ray.origin.x + ray.origin.y * ray.origin.y - radius.getValue() * radius.getValue();

	// Solve quadratic equation for _t_ values
	double t0, t1;
    if (!gf::Quadratic(A, B, C, &t0, &t1)) return false;

	// Compute intersection distance along ray
    if (t0 > ray.tMax || t1 < ray.tMin) return false;
    double thit = (t0 > ray.tMin)? t0 : t1 ;
    if (thit > ray.tMax) return false;

   //Compute possible cylinder hit position and $\phi
    Point3D hitPoint = ray(thit);
    double phi = atan2(hitPoint.y, hitPoint.x);
    if (phi < 0.) phi += gc::TwoPi;

	//Evaluate Tolerance
    double tol = 1e-5;
    double zmin = 0.;
	double zmax = length.getValue();

	// Test intersection against clipping parameters
    if( (thit - ray.tMin) < tol  || hitPoint.z < zmin || hitPoint.z > zmax || phi > phiMax.getValue() )
	{
        if (thit == t1) return false;
        if (t1 > ray.tMax) return false;
		thit = t1;

        hitPoint = ray(thit);
		phi = atan2( hitPoint.y, hitPoint.x );
		if ( phi < 0. ) phi += gc::TwoPi;
        if ( (thit - ray.tMin) < tol  || hitPoint.z < zmin || hitPoint.z > zmax || phi > phiMax.getValue() ) return false;
	}
	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
    if (tHit == 0 && dg == 0)
        return true;
    else if (tHit == 0 || dg == 0)
        gf::SevereError("Function Cylinder::Intersect(...) called with null pointers");


	// Find parametric representation of Cylinder hit
	double u = phi / phiMax.getValue();
	double v = hitPoint.z /length.getValue();

	// Compute cylinder \dpdu and \dpdv
	//double zradius = sqrt( hitPoint.x*hitPoint.x + hitPoint.y*hitPoint.y );
	//double invzradius = 1.0 / zradius;

	Vector3D dpdu( -phiMax.getValue() * radius.getValue() * sin ( phiMax.getValue() * u ),
						phiMax.getValue() * radius.getValue() * cos( phiMax.getValue() * u ),
                        0.);
    Vector3D dpdv(0., 0., length.getValue());


    Vector3D N = Normalize(CrossProduct(dpdu, dpdv));


    *dg = DifferentialGeometry(hitPoint, u, v, dpdu, dpdv, N, this);
    dg->shapeFrontSide = DotProduct(N, ray.direction()) <= 0.;

    *tHit = thit;

	return true;
}

Point3D ShapeCylinder::GetPoint3D(double u, double v) const
{
    if ( OutOfRange(u, v) )
        gf::SevereError("Function Function Poligon::GetPoint3D called with invalid parameters");

    double phi = u*phiMax.getValue();
    double ilength = v*length.getValue();

    double x = radius.getValue()*cos(phi);
    double y = radius.getValue()*sin(phi);
	double z = ilength;

    return Point3D(x, y, z);
}

Vector3D ShapeCylinder::GetNormal(double u, double /* v */) const
{
    double phi = u*phiMax.getValue();
    double x = cos(phi);
    double y = sin(phi);
    return Vector3D(x, y, 0.);
}

void ShapeCylinder::generatePrimitives(SoAction* action)
{
    generateQuads(action, QSize(48, 3), activeSide.getValue() == Side::INSIDE, activeSide.getValue() != Side::INSIDE);
}
