#include <vector>
#include <algorithm>

#include <QString>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include "libraries/geometry/gf.h"

#include "libraries/geometry/BBox.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/Ray.h"
#include "ShapeParabolicRectangle.h"
#include "libraries/geometry/Vector3D.h"


SO_NODE_SOURCE(ShapeParabolicRectangle)


void ShapeParabolicRectangle::initClass()
{
    SO_NODE_INIT_CLASS(ShapeParabolicRectangle, ShapeAbstract, "ShapeAbstract");
}

ShapeParabolicRectangle::ShapeParabolicRectangle()
{
    SO_NODE_CONSTRUCTOR(ShapeParabolicRectangle);

    SO_NODE_ADD_FIELD( focus, (1.) );
    SO_NODE_ADD_FIELD( widthX, (1.) );
    SO_NODE_ADD_FIELD( widthZ, (1.) );

    SO_NODE_DEFINE_ENUM_VALUE(Side, INSIDE);
    SO_NODE_DEFINE_ENUM_VALUE(Side, OUTSIDE);
    SO_NODE_SET_SF_ENUM_TYPE(activeSide, Side);
    SO_NODE_ADD_FIELD( activeSide, (OUTSIDE) );
}

BBox ShapeParabolicRectangle::getBox() const
{
	double xmin = -widthX.getValue()/2;
	double xmax = widthX.getValue()/2;

	double ymin = 0.0;
	double ymax = ( ( widthX.getValue() * widthX.getValue() ) + ( widthZ.getValue() * widthZ.getValue() ) )
			/ ( 16 * focus.getValue() );

	double zmin = -widthZ.getValue() / 2;
	double zmax = widthZ.getValue() / 2;

	return BBox( Point3D( xmin, ymin, zmin), Point3D( xmax, ymax, zmax) );
}

bool ShapeParabolicRectangle::intersect(const Ray& ray, double *tHit, DifferentialGeometry *dg) const
{
    double f = focus.getValue();
	double wX = widthX.getValue();
	double wZ = widthZ.getValue();

	// Compute quadratic coefficients
    double A = ray.direction().x * ray.direction().x + ray.direction().z * ray.direction().z;
    double B = 2. * ( ray.direction().x * ray.origin.x + ray.direction().z * ray.origin.z  - 2 * f * ray.direction().y );
    double C = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z - 4 * f * ray.origin.y;

	// Solve quadratic equation for _t_ values
	double t0, t1;
    if (!gf::Quadratic(A, B, C, &t0, &t1)) return false;

	// Compute intersection distance along ray
    if (t0 > ray.tMax || t1 < ray.tMin) return false;
    double thit =  t0 > ray.tMin ? t0 : t1;
    if (thit > ray.tMax) return false;

    double tolerance = 1e-5;

	//Compute possible hit position
    Point3D hitPoint = ray(thit);

	// Test intersection against clipping parameters
    if (thit - ray.tMin < tolerance || abs(hitPoint.x) > wX / 2 || abs(hitPoint.z) > wZ / 2)
	{
        if (thit == t1) return false;
        if (t1 > ray.tMax) return false;
		thit = t1;

        hitPoint = ray(thit);
        if (thit - ray.tMin < tolerance ||  abs(hitPoint.x) > wX / 2 || abs(hitPoint.z) > wZ / 2)
            return false;
	}

    // Now check if the function is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
    if (tHit == 0 && dg == 0)
        return true;
    else if (tHit == 0 || dg == 0)
        gf::SevereError( "Function ParabolicCyl::Intersect(...) called with null pointers" );

	// Compute possible parabola hit position

	// Find parametric representation of paraboloid hit
    double u = hitPoint.x/wX + 0.5;
    double v = hitPoint.z/wZ + 0.5;

    Vector3D dpdu(wX, ((-0.5 + u)*wX*wX)/(2.*f), 0.);
    Vector3D dpdv(0., ((-0.5 + v)*wZ*wZ)/(2.*f), wZ);

    Vector3D N = Normalize(cross(dpdu, dpdv));

    *dg = DifferentialGeometry(hitPoint, u, v, dpdu, dpdv, N, this);
    dg->shapeFrontSide = dot(N, ray.direction()) <= 0;

	*tHit = thit;
	return true;
}

Point3D ShapeParabolicRectangle::getPoint(double u, double v) const
{
    if (isInside(u, v) )
        gf::SevereError("Function Poligon::GetPoint3D called with invalid parameters");

    double x = (u - 0.5)*widthX.getValue();
    double z = (v - 0.5)*widthZ.getValue();
    double y = (x*x + z*z)/(4.*focus.getValue());
    return Point3D(x, y, z);
}

Vector3D ShapeParabolicRectangle::getNormal(double u, double v) const
{
	Vector3D dpdu( widthX.getValue(), ( (-0.5 + u) * widthX.getValue() *  widthX.getValue() )/(2 * focus.getValue()), 0 );
	Vector3D dpdv( 0.0, (( -0.5 + v) * widthZ.getValue() *  widthZ.getValue() ) /( 2 * focus.getValue() ), widthZ.getValue() );
    return Normalize(cross(dpdu, dpdv));
}

void ShapeParabolicRectangle::generatePrimitives(SoAction* action)
{
    double q = 2.*focus.getValue()*gc::Degree;
    int rows = 1 + ceil(widthX.getValue()/q);
    int columns = 1 + ceil(widthZ.getValue()/q);

    if (rows > 36) rows = 36;
    if (columns > 36) columns = 36;

    generateQuads(action, QSize(rows, columns), activeSide.getValue() == Side::OUTSIDE);
}
