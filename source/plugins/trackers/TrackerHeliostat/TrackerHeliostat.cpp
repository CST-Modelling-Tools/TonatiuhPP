#include "TrackerHeliostat.h"

#include <cmath>
#include <QString>
#include <Inventor/nodes/SoTransform.h>

#include "libraries/geometry/NormalVector.h"
#include "libraries/geometry/Point3D.h"
#include "libraries/geometry/Transform.h"
#include "libraries/geometry/Vector3D.h"
#include "kernel/scene/TSceneKit.h"
#include "kernel/trf.h"


SO_NODEENGINE_SOURCE(TrackerHeliostat)

void TrackerHeliostat::initClass()
{
    SO_NODEENGINE_INIT_CLASS(TrackerHeliostat, TTrackerForAiming, "TTrackerForAiming");
}

TrackerHeliostat::TrackerHeliostat():
    m_previousAimingPointType(0)
{
    SO_NODEENGINE_CONSTRUCTOR(TrackerHeliostat);

    SO_NODE_ADD_FIELD( primaryAxis, (0.f, 1.f, 0.f) );
    SO_NODE_ADD_FIELD( secondaryAxis, (1.f, 0.f, 0.f) );
    SO_NODE_ADD_FIELD( mirrorNormal, (0.f, 1.f, 0.f) );

    SO_NODE_ADD_FIELD( isAimingAbsolute, (TRUE) );
    SO_NODE_ADD_FIELD( aimingPoint, (0.f, 1.f, 0.f) );

    SO_NODEENGINE_ADD_OUTPUT(outputTranslation, SoSFVec3f);
    SO_NODEENGINE_ADD_OUTPUT(outputRotation, SoSFRotation);
    SO_NODEENGINE_ADD_OUTPUT(outputScaleFactor, SoSFVec3f);
    SO_NODEENGINE_ADD_OUTPUT(outputScaleOrientation, SoSFRotation);
    SO_NODEENGINE_ADD_OUTPUT(outputCenter, SoSFVec3f);

//    SO_NODE_ADD_FIELD( m_azimuth, (0.) );// for copy and paste
//    SO_NODE_ADD_FIELD( m_zenith, (90.) );
}

inline double findAngle(Vector3D& a, Vector3D& m, Vector3D& v, double av)
{
    return atan2(dot(a, cross(m, v)), dot(m, v) - av*av);
}

Vector3D findAngles(
    Vector3D& a, Vector3D& b,
    Vector3D& v, Vector3D& m, Vector3D& v0,
    double av, double bv0
)
{
    double alpha = findAngle(a, m, v, av);
    double beta = findAngle(b, v0, m, bv0);
    return Vector3D(alpha, beta, abs(alpha) + abs(beta));
}

void TrackerHeliostat::Evaluate(const Vector3D& vSunW, const Transform& transformWtO)
{
    // normal
    Vector3D vSun = transformWtO(vSunW);
    vSun.normalize();

    Point3D rAim = tgf::makePoint3D(aimingPoint.getValue());
    if (isAimingAbsolute.getValue())
        rAim = transformWtO(rAim);
    Vector3D vAim(rAim);
    if (!vAim.normalize()) return;

    Vector3D v = vSun + vAim;
    if (!v.normalize()) return;

    // cache
    Vector3D a = tgf::makeVector3D(primaryAxis.getValue());
    Vector3D b = tgf::makeVector3D(secondaryAxis.getValue());
    Vector3D v0 = tgf::makeVector3D(mirrorNormal.getValue());
    a.normalize();
    b.normalize();
    v0.normalize();

    Vector3D k = cross(a, b);
    double k2 = k.norm2();
    double ab = dot(a, b);
    double det = 1. - ab*ab;
    double av = dot(a, v);
    double bv0 = dot(b, v0);

    // algorithm
    if (qFuzzyIsNull(det)) return;
    double ma = (av - ab*bv0)/det;
    double mb = (bv0 - ab*av)/det;
    double mk = 1. - ma*ma - mb*mb - 2.*ma*mb*ab;
    if (mk < 0.) return;
    mk = sqrt(mk/k2);
    Vector3D m0 = ma*a + mb*b;

    Vector3D m1 = m0 - mk*k;
    Vector3D sol1 = findAngles(a, b, v, m1, v0, av, bv0);
    Vector3D m2 = m0 + mk*k;
    Vector3D sol2 = findAngles(a, b, v, m2, v0, av, bv0);

    // select smallest rotation
    Vector3D* sol;
    if (sol1.z < sol2.z)
        sol = &sol1;
    else
        sol = &sol2;

    SbRotation rotationA(primaryAxis.getValue(), sol->x);
    SbRotation rotationB(secondaryAxis.getValue(), sol->y);
    SbRotation rotation = rotationB*rotationA;

    SetEngineOutputRotation(rotation);
//    SO_ENGINE_OUTPUT(outputRotation, SoSFRotation, setValue(rotation) );
}
