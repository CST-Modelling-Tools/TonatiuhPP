#include "TrackerSingle.h"

#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodes/SoGroup.h>

#include "kernel/trf.h"
#include "TrackerSingleSolver.h"
#include "kernel/scene/TSeparatorKit.h"


SO_NODE_SOURCE(TrackerSingle)


void TrackerSingle::initClass()
{
    SO_NODE_INIT_CLASS(TrackerSingle, Tracker, "TrackerAbstract");
}

TrackerSingle::TrackerSingle()
{
    SO_NODE_CONSTRUCTOR(TrackerSingle);

    SO_NODE_ADD_FIELD( primaryShift, (0.f, 0.f, 1.f) );
    SO_NODE_ADD_FIELD( primaryAxis, (0.f, 1.f, 0.f) );
    SO_NODE_ADD_FIELD( primaryAngles, (-90*gcf::degree, 90*gcf::degree) );

    SO_NODE_ADD_FIELD( facetShift, (0.f, 0.f, 0.f) );
    SO_NODE_ADD_FIELD( facetNormal, (0.f, 0.f, 1.f) );

    SO_NODE_DEFINE_ENUM_VALUE(AimingFrame, global);
    SO_NODE_DEFINE_ENUM_VALUE(AimingFrame, primary);
    SO_NODE_SET_SF_ENUM_TYPE(aimingFrame, AimingFrame);
    SO_NODE_ADD_FIELD( aimingFrame, (primary) );
    SO_NODE_ADD_FIELD( aimingPoint, (0.f, 0.f, 1.f) );

    SO_NODE_ADD_FIELD( angleDefault, (0.f) );

    m_trough = 0;
    m_sensor = new SoNodeSensor(onModified, this);
    m_sensor->attach(this);
    onModified(this, 0);
}

TrackerSingle::~TrackerSingle()
{
    delete m_sensor;
    delete m_trough;
}

void TrackerSingle::update(TSeparatorKit* parent, const Transform& toGlobal, const vec3d& vSun)
{
    Transform toLocal = toGlobal.inversed();
    vec3d vSunL = toLocal.transformVector(vSun);
    vec3d rAim = tgf::makeVector3D(aimingPoint.getValue());

    double angle;
    if (aimingFrame.getValue() == global) {
        rAim = toLocal.transformPoint(rAim);
        angle = m_trough->solveReflectionGlobal(vSunL, rAim);
    } else if (aimingFrame.getValue() == primary) {
        angle = m_trough->solveReflectionPrimary(vSunL, rAim);
    } else {
        angle = 0;
    }
    angle = m_trough->selectSolution(angle);

    // rotate nodes
//    auto node = static_cast<TSeparatorKit*>(parent->getPart("group[0]", false));
    SoGroup* childList = (SoGroup*) parent->getPart("group", false);
    auto node = static_cast<TSeparatorKit*>(childList->getChild(0));
    if (!node) return;
    SoTransform* tPrimary = (SoTransform*) node->getPart("transform", true);
    tPrimary->translation = primaryShift.getValue();
    tPrimary->rotation.setValue(primaryAxis.getValue(), angle);
}

void TrackerSingle::onModified(void* data, SoSensor*)
{
    TrackerSingle* tracker = (TrackerSingle*) data;
    if (tracker->m_trough) delete tracker->m_trough;

    vec2d pa = tgf::makeVector2D(tracker->primaryAngles.getValue());

    tracker->m_trough = new TrackerSingleSolver(
        TrackingDrive(
            tgf::makeVector3D(tracker->primaryShift.getValue()),
            tgf::makeVector3D(tracker->primaryAxis.getValue()),
            IntervalPeriodic(pa.x, pa.y)
        ),
        TrackingVertex(
            tgf::makeVector3D(tracker->facetShift.getValue()),
            tgf::makeVector3D(tracker->facetNormal.getValue())
        )
    );
}
