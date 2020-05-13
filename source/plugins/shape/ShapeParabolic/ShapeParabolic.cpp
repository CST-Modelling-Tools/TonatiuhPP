#include "ShapeParabolic.h"

#include "kernel/shape/DifferentialGeometry.h"
#include "libraries/geometry/gf.h"
#include "libraries/geometry/BoundingBox.h"
#include "libraries/geometry/Ray.h"


SO_NODE_SOURCE(ShapeParabolic)


void ShapeParabolic::initClass()
{
    SO_NODE_INIT_CLASS(ShapeParabolic, ShapeAbstract, "ShapeAbstract");
}

ShapeParabolic::ShapeParabolic()
{
    SO_NODE_CONSTRUCTOR(ShapeParabolic);

    SO_NODE_ADD_FIELD( focusX, (1.) );
    SO_NODE_ADD_FIELD( focusY, (1.) );
    SO_NODE_ADD_FIELD( sizeX, (1.) );
    SO_NODE_ADD_FIELD( sizeY, (1.) );

    SO_NODE_DEFINE_ENUM_VALUE(Side, Back);
    SO_NODE_DEFINE_ENUM_VALUE(Side, Front);
    SO_NODE_SET_SF_ENUM_TYPE(activeSide, Side);
    SO_NODE_ADD_FIELD( activeSide, (Front) );
}

BoundingBox ShapeParabolic::getBox() const
{
    double xMax = sizeX.getValue()/2.;
    double yMax = sizeY.getValue()/2.;
    double zMax = (xMax*xMax/focusX.getValue() + yMax*yMax/focusY.getValue())/4.;

    return BoundingBox(
        Point3D(-xMax, -yMax, 0.),
        Point3D(xMax, yMax, zMax)
    );
}

bool ShapeParabolic::intersect(const Ray& ray, double *tHit, DifferentialGeometry *dg) const
{
    double gX = 1./focusX.getValue();
    double gY = 1./focusY.getValue();

    // intersection with full shape
    // (x0 + t*d_x)^2*gX + (y0 + t*d_y)^2*gY = 4*(z0 + t*d_z)
    double A = ray.direction().x*ray.direction().x*gX + ray.direction().y*ray.direction().y*gY;
    double B = 2.*(ray.direction().x*ray.origin.x*gX + ray.direction().y*ray.origin.y*gY) - 4.*ray.direction().z;
    double C = ray.origin.x*ray.origin.x*gX + ray.origin.y*ray.origin.y*gY - 4.*ray.origin.z;
    double ts[2];
    if (!gf::solveQuadratic(A, B, C, &ts[0], &ts[1])) return false;


    // intersection with clipped shape
    double raytMin = ray.tMin + 1e-5;
    for (int i = 0; i < 2; ++i)
    {
        double t = ts[i];
        if (t < raytMin || t > ray.tMax) continue;

        Point3D pHit = ray(t);
        if (2.*abs(pHit.x) > sizeX.getValue() || 2.*abs(pHit.y) > sizeY.getValue())
            continue;

        // differential geometry
        if (tHit == 0 && dg == 0)
            return true;
        else if (tHit == 0 || dg == 0)
            gf::SevereError("Function Parabolic::Intersect(...) called with null pointers");

        Vector3D dpdu(1., 0., pHit.x*gX/2.);
        Vector3D dpdv(0., 1., pHit.y*gY/2.);
        Vector3D normal(-pHit.x*gX, -pHit.y*gY, 2.);
        normal.normalize();

        bool isFront = dot(normal, ray.direction()) <= 0.;
        *dg = DifferentialGeometry(pHit, pHit.x, pHit.y, dpdu, dpdv, normal, this, isFront);

        *tHit = t;
        return true;
    }
    return false;
}

Vector3D ShapeParabolic::getPoint(double u, double v) const
{
    double x = (u - 0.5)*sizeX.getValue();
    double y = (v - 0.5)*sizeY.getValue();
    double z = (x*x/focusX.getValue() + y*y/focusY.getValue())/4.;
    return Vector3D(x, y, z);
}

Vector3D ShapeParabolic::getNormal(double u, double v) const
{
    double x = (u - 0.5)*sizeX.getValue();
    double y = (v - 0.5)*sizeY.getValue();
    Vector3D n(-x/focusX.getValue(), -y/focusY.getValue(), 2.);
    return n.normalized();
}

void ShapeParabolic::generatePrimitives(SoAction* action)
{
    double s;

    s = sizeX.getValue()/(360*gc::Degree*2.*focusX.getValue());
    if (s > 1.) s = 1.;
    int rows = 1 + ceil(48*s);

    s = sizeY.getValue()/(360*gc::Degree*2.*focusY.getValue());
    if (s > 1.) s = 1.;
    int columns = 1 + ceil(48*s);

    generateQuads(action, QSize(rows, columns), activeSide.getValue() == Side::Back);
}
