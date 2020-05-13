#include "TrackerTrough.h"

#include <cmath>

#include <Inventor/nodes/SoTransform.h>

#include "libraries/geometry/NormalVector.h"
#include "libraries/geometry/Point3D.h"
#include "kernel/trf.h"
#include "libraries/geometry/Transform.h"
#include "kernel/scene/TSceneKit.h"
#include "kernel/scene/TSeparatorKit.h"
#include "libraries/geometry/Vector3D.h"


SO_NODE_SOURCE(TrackerTrough)

void TrackerTrough::initClass()
{
    SO_NODE_INIT_CLASS(TrackerTrough, TrackerAbstract, "TrackerAbstract");
}

TrackerTrough::TrackerTrough()
{
    SO_NODE_CONSTRUCTOR(TrackerTrough);

    SO_NODE_ADD_FIELD( trackingAxis, (0.f, 1.f, 0.f) );
    SO_NODE_ADD_FIELD( mirrorNormal, (0.f, 0.f, 1.f) );

    SO_NODE_ADD_FIELD( isAimingAbsolute, (FALSE) );
    SO_NODE_ADD_FIELD( aimingPoint, (0.f, 0.f, 1.f) );
}

// rotation around a from m to v
inline double findAngle(Vector3D& a, Vector3D& m, Vector3D& v, double av)
{
    return atan2(dot(a, cross(m, v)), dot(m, v) - av*av);
}

void TrackerTrough::Evaluate(SoBaseKit* parent, const Transform& toGlobal, const Vector3D& vSun)
{
    Vector3D a = tgf::makeVector3D(trackingAxis.getValue());
    a.normalize();
    Vector3D v0 = tgf::makeVector3D(mirrorNormal.getValue());
    v0 -= dot(v0, a)*a;
    v0.normalize();

    Transform toLocal = toGlobal.inversed();
    Vector3D vS = toLocal(vSun);
    vS -= dot(vS, a)*a;
    vS.normalize();

    Vector3D vT = tgf::makeVector3D(aimingPoint.getValue());
    if (isAimingAbsolute.getValue())
        vT = toLocal.transformPoint(vT);
    vT -= dot(vT, a)*a;
    vT.normalize();

    double angle;
    if (isAimingAbsolute.getValue()) {
        Vector3D v = vS + vT;
        if (!v.normalize()) return;
        angle = findAngle(a, v0, v, dot(a, v));
    } else {
        Vector3D vS0 = -(vT - 2.*dot(vT, v0)*v0);
        angle = findAngle(a, vS0, vS, dot(a, vS));
    }

    auto node = static_cast<SoBaseKit*>(parent->getPart("childList[0]", false));
    if (!node) return;
    SoTransform* tParent = (SoTransform*) node->getPart("transform", true);
    tParent->rotation.setValue(trackingAxis.getValue(), angle);
}
