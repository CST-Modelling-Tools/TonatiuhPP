#include "ShapeSphere.h"

//#include <QObject>

#include <Inventor/sensors/SoFieldSensor.h>

#include "libraries/geometry/gc.h"
#include "libraries/geometry/gf.h"
#include "libraries/geometry/BBox.h"
#include "libraries/geometry/Ray.h"

#include "kernel/shape/DifferentialGeometry.h"
#include "kernel/TonatiuhFunctions.h"

SO_NODE_SOURCE(ShapeSphere)



void ShapeSphere::initClass()
{
    SO_NODE_INIT_CLASS(ShapeSphere, ShapeAbstract, "ShapeAbstract");
}

ShapeSphere::ShapeSphere()
{
	SO_NODE_CONSTRUCTOR(ShapeSphere);

    SO_NODE_ADD_FIELD( radius, (1.) );
    SO_NODE_ADD_FIELD( phiMax, (gc::TwoPi) );
    SO_NODE_ADD_FIELD( alphaMin, (-gc::Pi/2.) );
    SO_NODE_ADD_FIELD( alphaMax, (gc::Pi/2.) );

	SO_NODE_DEFINE_ENUM_VALUE( Side, INSIDE );
	SO_NODE_DEFINE_ENUM_VALUE( Side, OUTSIDE );
	SO_NODE_SET_SF_ENUM_TYPE( activeSide, Side );
	SO_NODE_ADD_FIELD( activeSide, (OUTSIDE) );

    m_sensor_radius = new SoFieldSensor(update_radius, this);
//    m_sensor_radius->setPriority(1); // does not work
    m_sensor_radius->attach(&radius);
    m_sensor_phiMax = new SoFieldSensor(update_phiMax, this);
    m_sensor_phiMax->attach(&phiMax);
    m_sensor_alphaMin = new SoFieldSensor(update_alphaMin, this);
    m_sensor_alphaMin->attach(&alphaMin);
    m_sensor_alphaMax = new SoFieldSensor(update_alphaMax, this);
    m_sensor_alphaMax->attach(&alphaMax);
}

ShapeSphere::~ShapeSphere()
{
    delete m_sensor_radius;
    delete m_sensor_phiMax;
    delete m_sensor_alphaMin;
    delete m_sensor_alphaMax;
}

double ShapeSphere::getArea() const
{
    double r = radius.getValue();
    return 4.*gc::Pi*r*r;
}
double ShapeSphere::getVolume() const
{
    double r = radius.getValue();
    return 4.*gc::Pi*r*r*r/3.;
}

BBox ShapeSphere::getBox() const
{
    double r = radius.getValue();
    double pM = phiMax.getValue();
    double cosPhiMax = cos(pM);
    double sinPhiMax = sin(pM);
    double aMin = alphaMin.getValue();
    double aMax = alphaMax.getValue();

    double rhoMin = std::min(r*cos(aMin), r*cos(aMax));
    double rhoMax = aMin*aMax > 0. ? std::max(r*cos(aMin), r*cos(aMax)) : r;

    double xMin;
    if (pM > gc::Pi)
        xMin = -rhoMax;
    else if (pM > 0.5*gc::Pi)
        xMin = rhoMax*cosPhiMax;
    else
        xMin = rhoMin*cosPhiMax;

    double xMax = rhoMax;

    double yMin;
    if (pM > 1.5*gc::Pi)
        yMin = -rhoMax;
    else if (pM > gc::Pi)
        yMin = rhoMax*sinPhiMax;
    else
        yMin = 0.;

    double yMax;
    if (pM > 0.5*gc::Pi)
        yMax = rhoMax;
    else
        yMax = rhoMax*sinPhiMax;

    double zMin = r*sin(aMin);
    double zMax = r*sin(aMax);

    return BBox(
        Point3D(xMin, yMin, zMin),
        Point3D(xMax, yMax, zMax)
    );
}

bool ShapeSphere::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const
{
    Vector3D r0 = Vector3D(ray.origin);
    double r = radius.getValue();

    // intersection with full shape
    // |r0 + t*d|^2 = R^2  (local coordinates)
    // a*t^2 + b*t + c = 0
    double A = ray.direction().norm2();
    double B = 2.*dot(ray.direction(), r0);
    double C = r0.norm2() - r*r;
    double t0, t1;
    if (!gf::solveQuadratic(A, B, C, &t0, &t1)) return false;

    // intersection with clipped shape
    double tolerance = 1e-5;
    double raytMin = ray.tMin + tolerance;
    if (t0 > ray.tMax || t1 < raytMin) return false;
    double t = t0 > raytMin ? t0 : t1;
    if (t > ray.tMax) return false;

    Point3D pHit = ray(t);
    double phi = atan2(pHit.y, pHit.x);
    if (phi < 0.) phi += gc::TwoPi;
    double alpha = asin(tgf::clamp(pHit.z/r, -1., 1.));
    if (phi > phiMax.getValue() || alpha < alphaMin.getValue() || alpha > alphaMax.getValue())
    {
        if (t == t1) return false;
        t = t1;
        if (t > ray.tMax) return false;

        pHit = ray(t);
        phi = atan2(pHit.y, pHit.x);
        if (phi < 0.) phi += gc::TwoPi;
        alpha = asin(tgf::clamp(pHit.z/r, -1., 1.));
        if (phi > phiMax.getValue() || alpha < alphaMin.getValue() || alpha > alphaMax.getValue())
            return false;
    }

    // differential geometry
    if (tHit == 0 && dg == 0) return true;
    else if (tHit == 0 || dg == 0)
        gf::SevereError( "Function ShapeSphere::Intersect(...) called with null pointers" );

    Vector3D dpdu(
        -pHit.y,
        pHit.x,
        0.
    );

    Vector3D dpdv(
        -cos(phi)*pHit.z,
        -sin(phi)*pHit.z,
        r*cos(alpha)
    );

    Vector3D normal(
        pHit.x/r,
        pHit.y/r,
        pHit.z/r
    );

    *dg = DifferentialGeometry(pHit, phi, alpha, dpdu, dpdv, normal, this);
    dg->shapeFrontSide = dot(normal, ray.direction()) <= 0.;

    *tHit = t;
    return true;
}

Vector3D ShapeSphere::getPoint(double u, double v) const
{
    Vector3D p = getNormal(u, v);
    return radius.getValue()*p;
}

Vector3D ShapeSphere::getNormal(double u, double v) const
{
    double phi = u*phiMax.getValue();
    double alpha = (1. - v)*alphaMin.getValue() + v*alphaMax.getValue();

    Vector3D p(
        cos(phi)*cos(alpha),
        sin(phi)*cos(alpha),
        sin(alpha)
    );
    return p;
}

void ShapeSphere::generatePrimitives(SoAction* action)
{
    generateQuads(action, QSize(48, 24), activeSide.getValue() == Side::INSIDE, activeSide.getValue() == Side::INSIDE);
}

void ShapeSphere::update_radius(void* data, SoSensor*)
{
    ShapeSphere* shape = (ShapeSphere*) data;
    if (shape->radius.getValue() <= 0.)
        shape->radius.setValue(1.);
}

void ShapeSphere::update_phiMax(void* data, SoSensor*)
{
    ShapeSphere* shape = (ShapeSphere*) data;
    double phi = shape->phiMax.getValue();
    if (phi <= 0. || phi >= gc::TwoPi)
        shape->phiMax.setValue(gc::TwoPi);
}

void ShapeSphere::update_alphaMin(void* data, SoSensor*)
{
    ShapeSphere* shape = (ShapeSphere*) data;
    if (shape->alphaMin.getValue() < -gc::Pi/2.)
        shape->alphaMin.setValue(-gc::Pi/2.);
    if (shape->alphaMax.getValue() <= shape->alphaMin.getValue())
    {
        shape->alphaMin.setValue(-gc::Pi/2.);
        shape->alphaMax.setValue(gc::Pi/2.);
    }
}

void ShapeSphere::update_alphaMax(void* data, SoSensor*)
{
    ShapeSphere* shape = (ShapeSphere*) data;
    if (shape->alphaMax.getValue() > gc::Pi/2.)
        shape->alphaMax.setValue(gc::Pi/2.);
    if (shape->alphaMax.getValue() <= shape->alphaMin.getValue())
    {
        shape->alphaMin.setValue(-gc::Pi/2.);
        shape->alphaMax.setValue(gc::Pi/2.);
    }
}
