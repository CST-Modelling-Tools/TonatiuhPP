#include "TrackerHeliostat.h"

#include <cmath>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoNode.h>

#include "libraries/geometry/Point3D.h"
#include "libraries/geometry/Vector3D.h"
#include "libraries/geometry/Transform.h"
#include "kernel/trf.h"


SO_NODE_SOURCE(TrackerHeliostat)

void TrackerHeliostat::initClass()
{
    SO_NODE_INIT_CLASS(TrackerHeliostat, TrackerAbstract, "TrackerAbstract");
}

TrackerHeliostat::TrackerHeliostat()
{
    SO_NODE_CONSTRUCTOR(TrackerHeliostat);

    SO_NODE_ADD_FIELD( primaryAxis, (0.f, 0.f, -1.f) ); // azimuth
    SO_NODE_ADD_FIELD( secondaryAxis, (1.f, 0.f, 0.f) ); // elevation
    SO_NODE_ADD_FIELD( mirrorNormal, (0.f, 0.f, 1.f) ); // zenith

    SO_NODE_ADD_FIELD( isAimingAbsolute, (TRUE) );
    SO_NODE_ADD_FIELD( aimingPoint, (0.f, 0.f, 1.f) );
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

void TrackerHeliostat::Evaluate(SoNode* parent, const Transform& toGlobal, const Vector3D& vSun)
{
    // normal
    Transform toLocal = toGlobal.inversed();
    Vector3D vS = toLocal.transformVector(vSun);
    vS.normalize();

    Vector3D vT = tgf::makeVector3D(aimingPoint.getValue());
    if (isAimingAbsolute.getValue())
        vT = toLocal.transformPoint(vT);
    if (!vT.normalize()) return;

    Vector3D vN = vS + vT;
    if (!vN.normalize()) return;

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
    double av = dot(a, vN);
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
    Vector3D sol1 = findAngles(a, b, vN, m1, v0, av, bv0);
    Vector3D m2 = m0 + mk*k;
    Vector3D sol2 = findAngles(a, b, vN, m2, v0, av, bv0);

    // select smallest rotation
    Vector3D* sol = sol1.z < sol2.z ? &sol1 : &sol2;

    SbRotation rotationA(primaryAxis.getValue(), sol->x);
    SbRotation rotationB(secondaryAxis.getValue(), sol->y);
    SbRotation rotation = rotationB*rotationA;

    SoSeparatorKit* kit = (SoSeparatorKit*) parent;
    auto node = static_cast<SoBaseKit*>(kit->getPart("childList[0]", false));
    if (!node) return;
    SoTransform* tParent = (SoTransform*) node->getPart("transform", true);
    tParent->rotation = rotation;
}
