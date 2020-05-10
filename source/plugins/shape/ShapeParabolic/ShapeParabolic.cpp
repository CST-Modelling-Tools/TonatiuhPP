#include "ShapeParabolic.h"

#include <vector>
#include <algorithm>

#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/gf.h"
#include "libraries/geometry/BBox.h"
#include "libraries/geometry/Ray.h"


SO_NODE_SOURCE(ShapeParabolic)


void ShapeParabolic::initClass()
{
    SO_NODE_INIT_CLASS(ShapeParabolic, ShapeAbstract, "ShapeAbstract");
}

ShapeParabolic::ShapeParabolic()
{
    SO_NODE_CONSTRUCTOR(ShapeParabolic);

    SO_NODE_ADD_FIELD( focus, (1.) );
    SO_NODE_ADD_FIELD( widthX, (1.) );
    SO_NODE_ADD_FIELD( widthY, (1.) );

    SO_NODE_DEFINE_ENUM_VALUE(Side, INSIDE);
    SO_NODE_DEFINE_ENUM_VALUE(Side, OUTSIDE);
    SO_NODE_SET_SF_ENUM_TYPE(activeSide, Side);
    SO_NODE_ADD_FIELD( activeSide, (OUTSIDE) );
}

BBox ShapeParabolic::getBox() const
{
    double xMax = widthX.getValue()/2.;
    double xMin = -xMax;

    double yMax = widthY.getValue()/2.;
    double yMin = -yMax;

    double zMin = 0.;
    double zMax = (xMax*xMax+ yMax*yMax)/(4.*focus.getValue());

    return BBox(
        Point3D(xMin, yMin, zMin),
        Point3D(xMax, yMax, zMax)
    );
}

bool ShapeParabolic::intersect(const Ray& ray, double *tHit, DifferentialGeometry *dg) const
{
    double f = focus.getValue();
	double wX = widthX.getValue();
    double wY = widthY.getValue();

    // intersection with full shape
    // (x0 + t*d_x)^2 + (y0 + t*d_y)^2 = 4*f*(z0 + t*d_z)
    double A = ray.direction().x*ray.direction().x + ray.direction().y*ray.direction().y;
    double B = 2.*(ray.direction().x*ray.origin.x + ray.direction().y*ray.origin.y) - 4.*f*ray.direction().z;
    double C = ray.origin.x*ray.origin.x + ray.origin.y*ray.origin.y - 4.*f*ray.origin.z;
	double t0, t1;
    if (!gf::solveQuadratic(A, B, C, &t0, &t1)) return false;


    // intersection with clipped shape
    double tolerance = 1e-5;
    double raytMin = ray.tMin + tolerance;
    if (t0 > ray.tMax || t1 < raytMin) return false;
    double t = t0 > raytMin ? t0 : t1;
    if (t > ray.tMax) return false;


    Point3D pHit = ray(t);
    if (abs(pHit.x) > wX / 2 || abs(pHit.y) > wY / 2)
	{
        if (t == t1) return false;
        t = t1;
        if (t > ray.tMax) return false;

        pHit = ray(t);
        if (abs(pHit.x) > wX / 2 || abs(pHit.y) > wY / 2)
            return false;
	}

    // differential geometry
    if (tHit == 0 && dg == 0)
        return true;
    else if (tHit == 0 || dg == 0)
        gf::SevereError("Function ParabolicCyl::Intersect(...) called with null pointers");

    Vector3D dpdu(1., 0., pHit.x/(2.*f));
    Vector3D dpdv(0., 1., pHit.y/(2.*f));
    Vector3D normal(-pHit.x, -pHit.y, 2.*f);
    normal.normalize();

    *dg = DifferentialGeometry(pHit, pHit.x, pHit.y, dpdu, dpdv, normal, this);
    dg->shapeFrontSide = dot(normal, ray.direction()) <= 0;

    *tHit = t;
	return true;
}

Vector3D ShapeParabolic::getPoint(double u, double v) const
{
    double x = (u - 0.5)*widthX.getValue();
    double y = (v - 0.5)*widthY.getValue();
    double z = (x*x + y*y)/(4.*focus.getValue());
    return Vector3D(x, y, z);
}

Vector3D ShapeParabolic::getNormal(double u, double v) const
{
    double x = (u - 0.5)*widthX.getValue();
    double y = (v - 0.5)*widthY.getValue();
    Vector3D n(-x, -y, 2.*focus.getValue());
    return n.normalized();
}

void ShapeParabolic::generatePrimitives(SoAction* action)
{
    double q = 2.*focus.getValue()*gc::Degree;
    int rows = 1 + ceil(widthX.getValue()/q);
    if (rows > 36) rows = 36;
    int columns = 1 + ceil(widthY.getValue()/q);
    if (columns > 36) columns = 36;

    generateQuads(action, QSize(rows, columns), activeSide.getValue() == Side::OUTSIDE);
}
