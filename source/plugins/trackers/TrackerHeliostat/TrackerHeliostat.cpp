#include "TrackerHeliostat.h"

#include <cmath>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/sensors/SoNodeSensor.h>

#include "libraries/geometry/Transform.h"
#include "kernel/trf.h"

#include "HeliostatModel.h"
#include <algorithm>

SO_NODE_SOURCE(TrackerHeliostat)


void TrackerHeliostat::initClass()
{
    SO_NODE_INIT_CLASS(TrackerHeliostat, TrackerAbstract, "TrackerAbstract");
}

TrackerHeliostat::TrackerHeliostat()
{
    SO_NODE_CONSTRUCTOR(TrackerHeliostat);

    SO_NODE_ADD_FIELD( primaryShift, (0.f, 0.f, 1.f) );
    SO_NODE_ADD_FIELD( primaryAxis, (0.f, 0.f, -1.f) ); // azimuth

    SO_NODE_ADD_FIELD( secondaryShift, (0.f, 0.f, 0.1f) );
    SO_NODE_ADD_FIELD( secondaryAxis, (1.f, 0.f, 0.f) ); // elevation

    SO_NODE_ADD_FIELD( mirrorPoint, (0.f, 0.f, 0.f) );
    SO_NODE_ADD_FIELD( mirrorNormal, (0.f, 0.f, 1.f) );

    SO_NODE_DEFINE_ENUM_VALUE(AimingType, Local);
    SO_NODE_DEFINE_ENUM_VALUE(AimingType, Global);
    SO_NODE_SET_SF_ENUM_TYPE(aimingType, AimingType);
    SO_NODE_ADD_FIELD( aimingType, (Global) );

    SO_NODE_ADD_FIELD( aimingPoint, (0.f, 0.f, 10.f) );

    m_hm = 0;
    m_sensor = new SoNodeSensor(update, this);
    m_sensor->attach(this);
    update(this, 0);
}


TrackerHeliostat::~TrackerHeliostat()
{
    delete m_sensor;
    delete m_hm;
}

void TrackerHeliostat::update(SoBaseKit* parent, const Transform& toGlobal, const Vector3D& vSun)
{
    QVector<Vector3D> angles;
    Transform toLocal = toGlobal.inversed();
    Vector3D vSunL = toLocal.transformVector(vSun);
    Vector3D rAim = tgf::makeVector3D(aimingPoint.getValue());
    if (aimingType.getValue() == Local) {
        angles = m_hm->solveReflectionLocal(vSunL, rAim);
    } else {
        rAim = toLocal.transformPoint(rAim);
        angles = m_hm->solveReflectionGlobal(vSunL, rAim);
    }

    if (angles.empty()) return;
    std::sort(angles.begin(), angles.end(),
    [](const Vector3D& a, const Vector3D& b) -> bool {
        return a.z < b.z;
    });
    Vector3D* sol = &angles[0];

    auto nodePrimary = static_cast<SoBaseKit*>(parent->getPart("childList[0]", false));
    if (!nodePrimary) return;
    SoTransform* tPrimary = (SoTransform*) nodePrimary->getPart("transform", true);
    tPrimary->translation = primaryShift.getValue();
    tPrimary->rotation.setValue(primaryAxis.getValue(), sol->x);

    auto nodeSecondary = static_cast<SoBaseKit*>(nodePrimary->getPart("childList[0]", false));
    if (!nodeSecondary) return;
    SoTransform* tSecondary = (SoTransform*) nodeSecondary->getPart("transform", true);
    tSecondary->translation = secondaryShift.getValue();
    tSecondary->rotation.setValue(secondaryAxis.getValue(), sol->y);
}

void TrackerHeliostat::update(void* data, SoSensor*)
{
    TrackerHeliostat* tracker = (TrackerHeliostat*) data;

    if (tracker->m_hm) delete tracker->m_hm;

    tracker->m_hm = new HeliostatModel(
        HeliostatDrive(
            tgf::makeVector3D(tracker->primaryShift.getValue()),
            tgf::makeVector3D(tracker->primaryAxis.getValue())
        ),
        HeliostatDrive(
            tgf::makeVector3D(tracker->secondaryShift.getValue()),
            tgf::makeVector3D(tracker->secondaryAxis.getValue())
        ),
        Vertex3D(
            tgf::makeVector3D(tracker->mirrorPoint.getValue()),
            tgf::makeVector3D(tracker->mirrorNormal.getValue())
        )
    );
}
