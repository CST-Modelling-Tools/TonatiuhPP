#include "TrackerDual.h"

#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodes/SoGroup.h>

#include "kernel/scene/TSeparatorKit.h"
#include "kernel/trf.h"
#include "TrackerDualSolver.h"

SO_NODE_SOURCE(TrackerDual)


void TrackerDual::initClass()
{
    SO_NODE_INIT_CLASS(TrackerDual, Tracker, "TrackerAbstract");
}

TrackerDual::TrackerDual()
{
    SO_NODE_CONSTRUCTOR(TrackerDual);

    SO_NODE_ADD_FIELD( primaryShift, (0.f, 0.f, 1.f) );
    SO_NODE_ADD_FIELD( primaryAxis, (0.f, 0.f, -1.f) ); // azimuth
    SO_NODE_ADD_FIELD( primaryAngles, (-90*gcf::degree, 90*gcf::degree) );

    SO_NODE_ADD_FIELD( secondaryShift, (0.f, 0.f, 0.1f) );
    SO_NODE_ADD_FIELD( secondaryAxis, (1.f, 0.f, 0.f) ); // elevation
    SO_NODE_ADD_FIELD( secondaryAngles, (-90*gcf::degree, 90*gcf::degree) );

    SO_NODE_ADD_FIELD( facetShift, (0.f, 0.f, 0.f) );
    SO_NODE_ADD_FIELD( facetNormal, (0.f, 0.f, 1.f) );

    SO_NODE_DEFINE_ENUM_VALUE(AimingFrame, global);
    SO_NODE_DEFINE_ENUM_VALUE(AimingFrame, secondary);
    SO_NODE_SET_SF_ENUM_TYPE(aimingFrame, AimingFrame);
    SO_NODE_ADD_FIELD( aimingFrame, (global) );
    SO_NODE_ADD_FIELD( aimingPoint, (0.f, 0.f, 10.f) );

    SO_NODE_ADD_FIELD( anglesDefault, (0., 0.) );

    m_heliostat = 0;
    m_sensor = new SoNodeSensor(onModified, this);
    m_sensor->setPriority(0);
    m_sensor->attach(this);
    onModified(this, 0);
}

TrackerDual::~TrackerDual()
{
    delete m_sensor;
    delete m_heliostat;
}

void TrackerDual::update(TSeparatorKit* parent, const Transform& toGlobal, const vec3d& vSun)
{
    QVector<Angles> solutions;
    Transform toLocal = toGlobal.inversed();
    vec3d vSunL = toLocal.transformVector(vSun);
    vec3d rAim = tgf::makeVector3D(aimingPoint.getValue());
    if (aimingFrame.getValue() == global) {
        rAim = toLocal.transformPoint(rAim);
        solutions = m_heliostat->solveReflectionGlobal(vSunL, rAim);
    } else if (aimingFrame.getValue() == secondary) {
        solutions = m_heliostat->solveReflectionSecondary(vSunL, rAim);
    }
    Angles solution = m_heliostat->selectSolution(solutions);

    // rotate nodes
    SoGroup* childList = (SoGroup*) parent->getPart("group", false);
    auto nodePrimary = static_cast<TSeparatorKit*>(childList->getChild(0));
    if (!nodePrimary) return;
    SoTransform* tPrimary = (SoTransform*) nodePrimary->getPart("transform", true);
    tPrimary->translation = primaryShift.getValue();
    tPrimary->rotation.setValue(primaryAxis.getValue(), solution.x);

    childList = (SoGroup*) nodePrimary->getPart("group", false);
//    auto nodeSecondary = static_cast<TSeparatorKit*>(nodePrimary->getPart("group[0]", false));
    auto nodeSecondary = static_cast<TSeparatorKit*>(childList->getChild(0));
    if (!nodeSecondary) return;
    SoTransform* tSecondary = (SoTransform*) nodeSecondary->getPart("transform", true);
    tSecondary->translation = secondaryShift.getValue();
    tSecondary->rotation.setValue(secondaryAxis.getValue(), solution.y);
}

void TrackerDual::onModified(void* data, SoSensor*)
{
    TrackerDual* tracker = (TrackerDual*) data;
    if (tracker->m_heliostat) delete tracker->m_heliostat;

    vec2d pa = tgf::makeVector2D(tracker->primaryAngles.getValue());
    vec2d pb = tgf::makeVector2D(tracker->secondaryAngles.getValue());

    tracker->m_heliostat = new TrackerDualSolver(
        TrackingDrive(
            tgf::makeVector3D(tracker->primaryShift.getValue()),
            tgf::makeVector3D(tracker->primaryAxis.getValue()),
            IntervalPeriodic(pa.x, pa.y)
        ),
        TrackingDrive(
            tgf::makeVector3D(tracker->secondaryShift.getValue()),
            tgf::makeVector3D(tracker->secondaryAxis.getValue()),
            IntervalPeriodic(pb.x, pb.y)
        ),
        TrackingVertex(
            tgf::makeVector3D(tracker->facetShift.getValue()),
            tgf::makeVector3D(tracker->facetNormal.getValue())
        ),
        tgf::makeVector2D(tracker->anglesDefault.getValue())
    );
}
