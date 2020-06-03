#include "ShapeSphere.h"

#include <Inventor/sensors/SoNodeSensor.h>

#include "kernel/profiles/ProfileRT.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/Box3D.h"
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

    m_sensor = new SoNodeSensor(onSensor, this);
//    m_sensor->setPriority(0); // does not help
    m_sensor->attach(this);
}

ShapeSphere::~ShapeSphere()
{
    delete m_sensor;
}

Box3D ShapeSphere::getBox(ProfileRT* profile) const
{
    Box3D box = profile->getBox();
    double phiMin = gcf::TwoPi*box.pMin.x;
    double phiMax = gcf::TwoPi*box.pMax.x;
    double alphaMin = gcf::pi*gcf::clamp(box.pMin.y, -0.5, 0.5);
    double alphaMax = gcf::pi*gcf::clamp(box.pMax.y, -0.5, 0.5);

    double r = radius.getValue();
    double rMin = r*cos(alphaMin);
    double rMax = r*cos(alphaMax);
    if (rMin > rMax) std::swap(rMin, rMax);
    if (alphaMin <= 0. && 0. <= alphaMax)
        rMax = r;

    double xMin = cos(phiMin);
    double xMax = cos(phiMax);
    double yMin = sin(phiMin);
    double yMax = sin(phiMax);
    if (xMin > xMax) std::swap(xMin, xMax);
    if (yMin > yMax) std::swap(yMin, yMax);

    if (phiMin <= 0. && 0. <= phiMax)
        xMax = 1.;
    if (phiMin <= -gcf::pi || phiMax >= gcf::pi)
        xMin = -1.;
    if (phiMin <= 0.5*gcf::pi && 0.5*gcf::pi <= phiMax)
        yMax = 1.;
    if (phiMin <= -0.5*gcf::pi && -0.5*gcf::pi <= phiMax)
        yMin = -1.;

    xMax *= xMax > 0. ? rMax : rMin;
    xMin *= xMin > 0. ? rMin : rMax;

    yMax *= yMax > 0. ? rMax : rMin;
    yMin *= yMin > 0. ? rMin : rMax;

    double zMin = r*sin(alphaMin);
    double zMax = r*sin(alphaMax);

    return Box3D(
        Vector3D(xMin, yMin, zMin),
        Vector3D(xMax, yMax, zMax)
    );
}

bool ShapeSphere::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg, ProfileRT* profile) const
{
    const Vector3D& rayO = ray.origin;
    const Vector3D& rayD = ray.direction();
    double r = radius.getValue();

    // |r0 + t*d|^2 = R^2
    double A = rayD.norm2();
    double B = 2.*dot(rayD, rayO);
    double C = rayO.norm2() - r*r;
    double ts[2];
    if (!gcf::solveQuadratic(A, B, C, &ts[0], &ts[1])) return false;

    for (int i = 0; i < 2; ++i)
    {
        double t = ts[i];
        if (t < ray.tMin + 1e-5 || t > ray.tMax) continue;

        Vector3D pHit = ray.point(t);
        double phi = atan2(pHit.y, pHit.x);
        double alpha = asin(gcf::clamp(pHit.z/r, -1., 1.));
        double u = phi/gcf::TwoPi;
        double v = alpha/gcf::pi;
        if (!profile->isInside(u, v)) continue;

        if (tHit == 0 && dg == 0)
            return true;
        else if (tHit == 0 || dg == 0)
            gcf::SevereError("ShapeSphere::intersect");

        *tHit = t;
        dg->point = pHit;
        dg->u = u;
        dg->v = v;
        dg->dpdu = Vector3D(-pHit.y, pHit.x, 0.);
        dg->dpdv = Vector3D(-cos(phi)*pHit.z, -sin(phi)*pHit.z, r*cos(alpha));
        dg->normal = pHit/r;
        dg->shape = this;
        dg->isFront = dot(dg->normal, rayD) <= 0.;
        return true;
    }
    return false;
}

void ShapeSphere::updateShapeGL(TShapeKit* parent)
{
    ProfileRT* aperture = (ProfileRT*) parent->profileRT.getValue();
    Box3D box = aperture->getBox();
    Vector3D v = box.extent();

    double s = v.x;
    if (s > 1.) s = 1.;
    int rows = 1 + ceil(48*s);

    s = v.y;
    if (s > 1.) s = 1.;
    int columns = 1 + ceil(24*s);

    makeQuadMesh(parent, QSize(rows, columns));
}

Vector3D ShapeSphere::getPoint(double u, double v) const
{
    return radius.getValue()*getNormal(u, v);
}

Vector3D ShapeSphere::getNormal(double u, double v) const
{
    double phi = u*gcf::TwoPi;
    double alpha = v*gcf::pi;
    return Vector3D(
        cos(phi)*cos(alpha),
        sin(phi)*cos(alpha),
        sin(alpha)
    );
}

void ShapeSphere::onSensor(void* data, SoSensor*)
{
    ShapeSphere* shape = (ShapeSphere*) data;

    if (shape->radius.getValue() <= 0.)
        shape->radius.setValue(1.);

//    double phi = shape->phiMax.getValue();
//    if (phi <= 0. || phi > gcf::TwoPi)
//        shape->phiMax.setValue(gcf::TwoPi);

//    if (shape->alphaMin.getValue() < -gcf::pi/2.)
//        shape->alphaMin.setValue(-gcf::pi/2.);

//    if (shape->alphaMax.getValue() > gcf::pi/2.)
//        shape->alphaMax.setValue(gcf::pi/2.);

//    if (shape->alphaMax.getValue() <= shape->alphaMin.getValue())
//    {
//        shape->alphaMin.setValue(-gcf::pi/2.);
//        shape->alphaMax.setValue(gcf::pi/2.);
//    }
}
