#include "ShapeCylinder.h"

#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/gc.h"
#include "libraries/geometry/gf.h"
#include "libraries/geometry/BBox.h"
#include "libraries/geometry/Ray.h"
#include "libraries/geometry/Vector3D.h"


SO_NODE_SOURCE(ShapeCylinder)


void ShapeCylinder::initClass()
{
    SO_NODE_INIT_CLASS(ShapeCylinder, ShapeAbstract, "ShapeAbstract");
}

ShapeCylinder::ShapeCylinder()
{
    SO_NODE_CONSTRUCTOR(ShapeCylinder);

    SO_NODE_ADD_FIELD( radius, (0.5) );
    SO_NODE_ADD_FIELD( phiMax, (gc::TwoPi) );
    SO_NODE_ADD_FIELD( length, (1.) );

    SO_NODE_DEFINE_ENUM_VALUE(Side, INSIDE);
    SO_NODE_DEFINE_ENUM_VALUE(Side, OUTSIDE);
    SO_NODE_SET_SF_ENUM_TYPE(activeSide, Side);
    SO_NODE_ADD_FIELD( activeSide, (OUTSIDE) );
}

double ShapeCylinder::getArea() const
{
    return 2.*gc::Pi*radius.getValue()*length.getValue();
}

double ShapeCylinder::getVolume() const
{
    return gc::Pi*radius.getValue()*radius.getValue()*length.getValue();
}

BBox ShapeCylinder::getBox() const
{
    double r = radius.getValue();
    double pM = phiMax.getValue();
    double cosPhiMax = cos(pM);
    double sinPhiMax = sin(pM);

    double xMin;
    if (pM > gc::Pi)
        xMin = -r;
    else
        xMin = r*cosPhiMax;

    double xMax = r;

    double yMin;
    if (pM > 1.5*gc::Pi)
        yMin = -r;
    else if (pM > gc::Pi)
        yMin = r*sinPhiMax;
    else
        yMin = 0.;

    double yMax;
    if (pM > 0.5*gc::Pi)
        yMax = r;
    else
        yMax = r*sinPhiMax;

    double zMax = length.getValue()/2.;
    double zMin = -zMax;

    return BBox(
        Point3D(xMin, yMin, zMin),
        Point3D(xMax, yMax, zMax)
    );
}

bool ShapeCylinder::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const
{
    double r = radius.getValue();

    // intersection with full cylinder
    // |r0xy + dxy*t|^2 = R^2 (in local coordinates)
    double A = ray.direction().x*ray.direction().x + ray.direction().y*ray.direction().y;
    double B = 2.*(ray.direction().x*ray.origin.x + ray.direction().y*ray.origin.y);
    double C = ray.origin.x*ray.origin.x + ray.origin.y*ray.origin.y - r*r;
    double ts[2];
    if (!gf::solveQuadratic(A, B, C, &ts[0], &ts[1])) return false;

    // intersection with clipped shape
    double raytMin = ray.tMin + 1e-5;
    for (int i = 0; i < 2; ++i)
    {
        double t = ts[i];
        if (t < raytMin || t > ray.tMax) continue;

        Point3D pHit = ray(t);
        double phi = atan2(pHit.y, pHit.x);
        if (phi < 0.) phi += gc::TwoPi;

        if (phi > phiMax.getValue() ||  2.*abs(pHit.z) > length.getValue())
            continue;

        // differential geometry
        if (tHit == 0 && dg == 0)
            return true;
        else if (tHit == 0 || dg == 0)
            gf::SevereError("Function Cylinder::Intersect(...) called with null pointers");

        Vector3D dpdu(-pHit.y, pHit.x, 0.);
        Vector3D dpdv(0., 0., 1.);
        Vector3D normal(pHit.x/r, pHit.y/r, 0.);

        bool isFront = dot(normal, ray.direction()) <= 0.;
        *dg = DifferentialGeometry(pHit, phi, pHit.z, dpdu, dpdv, normal, this, isFront);

        *tHit = t;
        return true;
    }
    return false;
}

Vector3D ShapeCylinder::getPoint(double u, double v) const
{
    double phi = u*phiMax.getValue();
    double x = radius.getValue()*cos(phi);
    double y = radius.getValue()*sin(phi);
    double z = (v - 0.5)*length.getValue();
    return Vector3D(x, y, z);
}

Vector3D ShapeCylinder::getNormal(double u, double /*v*/) const
{
    double phi = u*phiMax.getValue();
    double x = cos(phi);
    double y = sin(phi);
    return Vector3D(x, y, 0.);
}

void ShapeCylinder::generatePrimitives(SoAction* action)
{
    generateQuads(action, QSize(48, 2), activeSide.getValue() == Side::INSIDE, activeSide.getValue() != Side::INSIDE);
}
