#include "ShapeSphere.h"

#include <Inventor/sensors/SoNodeSensor.h>

#include "kernel/shape/DifferentialGeometry.h"
#include "kernel/TonatiuhFunctions.h"
#include "libraries/geometry/gcf.h"
#include "libraries/geometry/gcf.h"
#include "libraries/geometry/BoundingBox.h"
#include "libraries/geometry/Ray.h"


SO_NODE_SOURCE(ShapeSphere)


void ShapeSphere::initClass()
{
    SO_NODE_INIT_CLASS(ShapeSphere, ShapeRT, "ShapeRT");
}

ShapeSphere::ShapeSphere()
{
	SO_NODE_CONSTRUCTOR(ShapeSphere);

    SO_NODE_ADD_FIELD( radius, (1.) );
    SO_NODE_ADD_FIELD( phiMax, (gcf::TwoPi) );
    SO_NODE_ADD_FIELD( alphaMin, (-gcf::pi/2.) );
    SO_NODE_ADD_FIELD( alphaMax, (gcf::pi/2.) );

    SO_NODE_DEFINE_ENUM_VALUE( Side, back );
    SO_NODE_DEFINE_ENUM_VALUE( Side, front );
	SO_NODE_SET_SF_ENUM_TYPE( activeSide, Side );
    SO_NODE_ADD_FIELD( activeSide, (front) );

    m_sensor = new SoNodeSensor(update, this);
    m_sensor->setPriority(1); // does not help
    m_sensor->attach(this);
}

ShapeSphere::~ShapeSphere()
{
    delete m_sensor;
}

double ShapeSphere::getArea() const
{
    double r = radius.getValue();
    return 4.*gcf::pi*r*r;
}
double ShapeSphere::getVolume() const
{
    double r = radius.getValue();
    return 4.*gcf::pi*r*r*r/3.;
}

BoundingBox ShapeSphere::getBox() const
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
    if (pM > gcf::pi)
        xMin = -rhoMax;
    else if (pM > 0.5*gcf::pi)
        xMin = rhoMax*cosPhiMax;
    else
        xMin = rhoMin*cosPhiMax;

    double xMax = rhoMax;

    double yMin;
    if (pM > 1.5*gcf::pi)
        yMin = -rhoMax;
    else if (pM > gcf::pi)
        yMin = rhoMax*sinPhiMax;
    else
        yMin = 0.;

    double yMax;
    if (pM > 0.5*gcf::pi)
        yMax = rhoMax;
    else
        yMax = rhoMax*sinPhiMax;

    double zMin = r*sin(aMin);
    double zMax = r*sin(aMax);

    return BoundingBox(
        Vector3D(xMin, yMin, zMin),
        Vector3D(xMax, yMax, zMax)
    );
}

bool ShapeSphere::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const
{
    const Vector3D& r0 = ray.origin;
    double r = radius.getValue();

    // intersection with full shape
    // |r0 + t*d|^2 = R^2  (local coordinates)
    // a*t^2 + b*t + c = 0
    double A = ray.direction().norm2();
    double B = 2.*dot(ray.direction(), r0);
    double C = r0.norm2() - r*r;
    double ts[2];
    if (!gcf::solveQuadratic(A, B, C, &ts[0], &ts[1])) return false;

    // intersection with clipped shape
    double raytMin = ray.tMin + 1e-5;
    for (int i = 0; i < 2; ++i)
    {
        double t = ts[i];
        if (t < raytMin || t > ray.tMax) continue;

        Vector3D pHit = ray.point(t);
        double phi = atan2(pHit.y, pHit.x);
        if (phi < 0.) phi += gcf::TwoPi;
        double alpha = asin(gcf::clamp(pHit.z/r, -1., 1.));

        if (phi > phiMax.getValue() || alpha < alphaMin.getValue() || alpha > alphaMax.getValue())
            continue;

        // differential geometry
        if (tHit == 0 && dg == 0)
            return true;
        else if (tHit == 0 || dg == 0)
            gcf::SevereError("Function ShapeSphere::Intersect(...) called with null pointers");

        *tHit = t;

        dg->point = pHit;
        dg->u = phi;
        dg->v = alpha;
        dg->dpdu = Vector3D(-pHit.y, pHit.x, 0.);
        dg->dpdv = Vector3D(-cos(phi)*pHit.z, -sin(phi)*pHit.z, r*cos(alpha));
        dg->normal = Vector3D(pHit)/r;
        dg->shape = this;
        dg->isFront = dot(dg->normal, ray.direction()) <= 0.;

        return true;
    }
    return false;
}

Vector3D ShapeSphere::getPoint(double u, double v) const
{
    return radius.getValue()*getNormal(u, v);
}

Vector3D ShapeSphere::getNormal(double u, double v) const
{
    double phi = u*phiMax.getValue();
    double alpha = (1. - v)*alphaMin.getValue() + v*alphaMax.getValue();

    return Vector3D(
        cos(phi)*cos(alpha),
        sin(phi)*cos(alpha),
        sin(alpha)
    );
}

void ShapeSphere::generatePrimitives(SoAction* action)
{
    generateQuads(action, QSize(48, 24), activeSide.getValue() == Side::back, activeSide.getValue() == Side::back);
}

void ShapeSphere::update(void* data, SoSensor*)
{
    ShapeSphere* shape = (ShapeSphere*) data;

    if (shape->radius.getValue() <= 0.)
        shape->radius.setValue(1.);

    double phi = shape->phiMax.getValue();
    if (phi <= 0. || phi >= gcf::TwoPi)
        shape->phiMax.setValue(gcf::TwoPi);

    if (shape->alphaMin.getValue() < -gcf::pi/2.)
        shape->alphaMin.setValue(-gcf::pi/2.);

    if (shape->alphaMax.getValue() > gcf::pi/2.)
        shape->alphaMax.setValue(gcf::pi/2.);

    if (shape->alphaMax.getValue() <= shape->alphaMin.getValue())
    {
        shape->alphaMin.setValue(-gcf::pi/2.);
        shape->alphaMax.setValue(gcf::pi/2.);
    }
}
