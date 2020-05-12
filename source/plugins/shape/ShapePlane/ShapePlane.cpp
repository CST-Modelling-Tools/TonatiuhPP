#include "ShapePlane.h"

#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/gf.h"
#include "libraries/geometry/BoundingBox.h"
#include "libraries/geometry/Ray.h"

SO_NODE_SOURCE(ShapePlane)


void ShapePlane::initClass()
{
    SO_NODE_INIT_CLASS(ShapePlane, ShapeAbstract, "ShapeAbstract");
}

ShapePlane::ShapePlane()
{
    SO_NODE_CONSTRUCTOR(ShapePlane);
    SO_NODE_ADD_FIELD( widthX, (1.) );
    SO_NODE_ADD_FIELD( widthY, (1.) );

    SO_NODE_DEFINE_ENUM_VALUE( Side, INSIDE );
    SO_NODE_DEFINE_ENUM_VALUE( Side, OUTSIDE );
    SO_NODE_SET_SF_ENUM_TYPE( activeSide, Side );
    SO_NODE_ADD_FIELD( activeSide, (OUTSIDE) );
}

double ShapePlane::getArea() const
{
    return widthX.getValue()*widthY.getValue();
}

BoundingBox ShapePlane::getBox() const
{
    double xMax = widthX.getValue()/2.;
    double yMax = widthY.getValue()/2.;
    double eps = 0.01*std::min(xMax, yMax);
    return BoundingBox(
        Point3D(-xMax, -yMax, -eps),
        Point3D(xMax, yMax, eps)
    );
}

bool ShapePlane::intersect(const Ray& ray, double *tHit, DifferentialGeometry* dg) const
{
    // intersection with full shape (in local coordinates)
    // r0_z + d_z*t = 0
    if (ray.origin.z == 0 && ray.direction().z == 0) return false;
    double t = -ray.origin.z*ray.invDirection().z;

    double tolerance = 1e-5;
    if (t < ray.tMin + tolerance || t > ray.tMax) return false;

    // intersection with clipped shape
    Point3D pHit = ray(t);
    if (2.*abs(pHit.x) > widthX.getValue() || 2.*abs(pHit.y) > widthY.getValue())
        return false;

    if (tHit == 0 && dg == 0) return true;
    else if (tHit == 0 || dg == 0)
        gf::SevereError( "Function Sphere::Intersect(...) called with null pointers" );

    Vector3D dpdu(1., 0., 0.);
    Vector3D dpdv(0., 1., 0.);
    Vector3D normal(0., 0., 1.);

    bool isFront = dot(normal, ray.direction()) <= 0.;
    *dg = DifferentialGeometry(pHit, pHit.x, pHit.y, dpdu, dpdv, normal, this, isFront);

    *tHit = t;
	return true;
}

Vector3D ShapePlane::getPoint(double u, double v) const
{
    double x = (u - 0.5)*widthX.getValue();
    double y = (v - 0.5)*widthY.getValue();
    return Vector3D(x, y, 0.);
}

Vector3D ShapePlane::getNormal(double /*u*/, double /*v*/) const
{
    return Vector3D(0., 0., 1.);
}

void ShapePlane::generatePrimitives(SoAction* action)
{
     generateQuads(action, QSize(2, 2), activeSide.getValue() == Side::INSIDE);
}
