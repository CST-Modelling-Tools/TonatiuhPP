#include <iostream>

#include <QMessageBox>
#include <QObject>
#include <QString>

#include <Inventor/sensors/SoFieldSensor.h>

#include "libraries/geometry/gc.h"
#include "libraries/geometry/gf.h"
#include "libraries/geometry/BBox.h"
#include "libraries/geometry/Ray.h"

#include "kernel/shape/DifferentialGeometry.h"
#include "ShapeSphere.h"

SO_NODE_SOURCE(ShapeSphere)



void ShapeSphere::initClass()
{
    SO_NODE_INIT_CLASS(ShapeSphere, ShapeAbstract, "ShapeAbstract");
}

ShapeSphere::ShapeSphere():
    m_radiusOld(0.5),
    m_yMaxOld(0.5),
    m_yMinOld(-0.5),
    m_sensor_radius(0),
    m_sensor_yMin(0),
    m_sensor_yMax(0),
    m_sensor_phiMax(0)
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

    m_sensor_radius = new SoFieldSensor(update_radius, this);
    m_sensor_radius->attach(&radius);
    m_sensor_yMin = new SoFieldSensor(update_yMin, this);
    m_sensor_yMin->attach(&yMin);
    m_sensor_yMax = new SoFieldSensor(update_yMax, this);
    m_sensor_yMax->attach(&yMax);
    m_sensor_phiMax = new SoFieldSensor(update_phiMax, this);
    m_sensor_phiMax->attach(&phiMax);
}

ShapeSphere::~ShapeSphere()
{
    delete m_sensor_radius;
    delete m_sensor_yMin;
    delete m_sensor_yMax;
    delete m_sensor_phiMax;
}

SoNode* ShapeSphere::copy(SbBool copyConnections) const
{
	// Use the standard version of the copy method to create
	// a copy of this instance, including its field data
    ShapeSphere* shape = dynamic_cast<ShapeSphere*>( SoNode::copy(copyConnections) );

    shape->m_yMaxOld = m_yMaxOld;
    shape->m_yMinOld = m_yMinOld;

    return shape;
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

    return BBox(
        Point3D(xmin, ymin, zmin),
        Point3D(xmax, ymax, zmax)
    );
}

bool ShapeSphere::intersect(const Ray& ray, double* tHit, DifferentialGeometry* dg) const
{
    // intersection with full shape
    // |r0 + t*d|^2 = R^2  (local coordinates)
    // a*t^2 + b*t + c = 0
    Vector3D rayOrigin = Vector3D(ray.origin);
    double r = radius.getValue();
    double A = ray.direction().lengthSquared();
    double B = 2. * dot( rayOrigin, ray.direction() );
    double C = rayOrigin.lengthSquared() - r*r;
    double t0, t1;
    if (!gf::Quadratic(A, B, C, &t0, &t1)) return false;

    // ray
    if (t0 > ray.tMax || t1 < ray.tMin) return false;
    double thit = ( t0 > ray.tMin )? t0 : t1;
    if (thit > ray.tMax) return false;

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
    if ( ( tHit == 0 ) && ( dg == 0 ) ) return true;
    else if( ( tHit == 0 ) || ( dg == 0 ) )
        gf::SevereError( "Function ShapeSphere::Intersect(...) called with null pointers" );

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

    *dg = DifferentialGeometry(hitPoint, u, v, dpdu, dpdv, N, this);
    dg->shapeFrontSide = dot(N, ray.direction()) <= 0.;

    *tHit = thit;
    return true;
}

Point3D ShapeSphere::getPoint(double u, double v) const
{
    if ( isInside(u, v) )
        gf::SevereError( "Function Poligon::GetPoint3D called with invalid parameters" );

    double alphaMin = acos(yMax.getValue()/radius.getValue());
    double alphaMax = acos(yMin.getValue()/radius.getValue());
    double alpha = alphaMin + u*(alphaMax - alphaMin);
    double gamma = v*phiMax.getValue();

    Point3D p(
        sin(gamma)*cos(alpha),
        cos(gamma)*cos(alpha),
        sin(alpha)
    );
    return radius.getValue()*p;
}

Vector3D ShapeSphere::getNormal(double u, double v) const
{
    double alphaMin = acos(yMax.getValue()/radius.getValue());
    double alphaMax = acos(yMin.getValue()/radius.getValue());
    double alpha = alphaMin + u*(alphaMax - alphaMin);
    double gamma = v*phiMax.getValue();

    Vector3D p(
        sin(gamma)*cos(alpha),
        cos(gamma)*cos(alpha),
        sin(alpha)
    );
    return p;
}

void ShapeSphere::generatePrimitives(SoAction* action)
{
    generateQuads(action, QSize(24, 48), activeSide.getValue() == Side::INSIDE, activeSide.getValue() == Side::INSIDE);
}

void ShapeSphere::update_radius(void* data, SoSensor*)
{
    ShapeSphere* shape = (ShapeSphere*) data;
    if (shape->radius.getValue() <= 0.) {
        QMessageBox::warning(0, "Tonatiuh",
            QObject::tr( "The sphere radius must be a positive value." ) );
        shape->radius.setValue(shape->m_radiusOld);
    } else if (shape->radius.getValue() < std::max(
        std::fabs(shape->yMin.getValue()),
        std::fabs(shape->yMax.getValue())
    )) {
        QMessageBox::warning( 0, "Tonatiuh",
            QObject::tr( "The sphere radius must equal or greater than y value." ) );
        shape->radius.setValue(shape->m_radiusOld);
    } else
        shape->m_radiusOld = shape->radius.getValue();
}

void ShapeSphere::update_yMin(void* data, SoSensor*)
{
    ShapeSphere* shape = (ShapeSphere *) data;
    if ( shape->yMin.getValue() >= shape->yMax.getValue() )
    {
        QMessageBox::warning( 0,
                QLatin1String( "Tonatiuh" ),
                QObject::tr( "Sphere y min must be smaller than y max value. ") );
        shape->yMin.setValue( shape->m_yMinOld );
    }
    else if( shape->yMin.getValue() < -shape->radius.getValue() )
    {
        shape->yMin.setValue( -shape->radius.getValue() );
        shape->m_yMinOld = -shape->radius.getValue();
    }
    else
        shape->m_yMinOld = shape->yMin.getValue();
}

void ShapeSphere::update_yMax(void* data, SoSensor*)
{
    ShapeSphere* shape = (ShapeSphere *) data;
    if ( shape->yMax.getValue() < shape->yMin.getValue() )
	{
        QMessageBox::warning(0, "Tonatiuh",
				QObject::tr( "Sphere y max must be larger than y min value. ") );
        shape->yMax.setValue( shape->m_yMaxOld );
    } else if( shape->yMax.getValue() > shape->radius.getValue() )
	{
        shape->yMax.setValue( shape->radius.getValue() );
        shape->m_yMaxOld = shape->radius.getValue();
    } else
        shape->m_yMaxOld = shape->yMax.getValue();
}

void ShapeSphere::update_phiMax(void* data, SoSensor*)
{
    ShapeSphere* shape = (ShapeSphere*) data;
    if (shape->phiMax.getValue() < 0.)
        shape->phiMax.setValue(0.);
    else if (shape->phiMax.getValue() > gc::TwoPi)
        shape->phiMax.setValue(gc::TwoPi);
}
