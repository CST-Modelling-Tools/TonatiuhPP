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
    SO_NODE_ADD_FIELD( primaryAngles, (-90*gcf::degree, 90*gcf::degree) );

    SO_NODE_ADD_FIELD( secondaryShift, (0.f, 0.f, 0.1f) );
    SO_NODE_ADD_FIELD( secondaryAxis, (1.f, 0.f, 0.f) ); // elevation
    SO_NODE_ADD_FIELD( secondaryAngles, (-90*gcf::degree, 90*gcf::degree) );

    SO_NODE_ADD_FIELD( mirrorPoint, (0.f, 0.f, 0.f) );
    SO_NODE_ADD_FIELD( mirrorNormal, (0.f, 0.f, 1.f) );

    SO_NODE_DEFINE_ENUM_VALUE(AimingType, local);
    SO_NODE_DEFINE_ENUM_VALUE(AimingType, global);
    SO_NODE_SET_SF_ENUM_TYPE(aimingFrame, AimingType);
    SO_NODE_ADD_FIELD( aimingFrame, (global) );

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
    QVector<Angles> solutions;
    Transform toLocal = toGlobal.inversed();
    Vector3D vSunL = toLocal.transformVector(vSun);
    Vector3D rAim = tgf::makeVector3D(aimingPoint.getValue());
    if (aimingFrame.getValue() == local) {
        solutions = m_hm->solveReflectionLocal(vSunL, rAim);
    } else {
        rAim = toLocal.transformPoint(rAim);
        solutions = m_hm->solveReflectionGlobal(vSunL, rAim);
    }

    Angles solution = m_hm->selectSolution(solutions);

    auto nodePrimary = static_cast<SoBaseKit*>(parent->getPart("childList[0]", false));
    if (!nodePrimary) return;
    SoTransform* tPrimary = (SoTransform*) nodePrimary->getPart("transform", true);
    tPrimary->translation = primaryShift.getValue();
    tPrimary->rotation.setValue(primaryAxis.getValue(), solution.x);

    auto nodeSecondary = static_cast<SoBaseKit*>(nodePrimary->getPart("childList[0]", false));
    if (!nodeSecondary) return;
    SoTransform* tSecondary = (SoTransform*) nodeSecondary->getPart("transform", true);
    tSecondary->translation = secondaryShift.getValue();
    tSecondary->rotation.setValue(secondaryAxis.getValue(), solution.y);
}

void TrackerHeliostat::update(void* data, SoSensor*)
{
    TrackerHeliostat* tracker = (TrackerHeliostat*) data;

    if (tracker->m_hm) delete tracker->m_hm;

    Vector2D pa = tgf::makeVector2D(tracker->primaryAngles.getValue());
    Vector2D pb = tgf::makeVector2D(tracker->secondaryAngles.getValue());

    tracker->m_hm = new HeliostatModel(
        HeliostatDrive(
            tgf::makeVector3D(tracker->primaryShift.getValue()),
            tgf::makeVector3D(tracker->primaryAxis.getValue()),
            IntervalAngular(pa.x, pa.y)
        ),
        HeliostatDrive(
            tgf::makeVector3D(tracker->secondaryShift.getValue()),
            tgf::makeVector3D(tracker->secondaryAxis.getValue()),
            IntervalAngular(pb.x, pb.y)
        ),
        Vertex3D(
            tgf::makeVector3D(tracker->mirrorPoint.getValue()),
            tgf::makeVector3D(tracker->mirrorNormal.getValue())
        )
    );
}
