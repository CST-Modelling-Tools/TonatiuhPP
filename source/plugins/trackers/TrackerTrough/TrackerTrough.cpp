#include "TrackerTrough.h"

#include <Inventor/sensors/SoNodeSensor.h>

#include "kernel/trf.h"
#include "TroughModel.h"

SO_NODE_SOURCE(TrackerTrough)


void TrackerTrough::initClass()
{
    SO_NODE_INIT_CLASS(TrackerTrough, TrackerAbstract, "TrackerAbstract");
}

TrackerTrough::TrackerTrough()
{
    SO_NODE_CONSTRUCTOR(TrackerTrough);

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

TrackerTrough::~TrackerTrough()
{
    delete m_sensor;
    delete m_trough;
}

void TrackerTrough::update(SoBaseKit* parent, const Transform& toGlobal, const Vector3D& vSun)
{
    Transform toLocal = toGlobal.inversed();
    Vector3D vSunL = toLocal.transformVector(vSun);
    Vector3D rAim = tgf::makeVector3D(aimingPoint.getValue());

    double angle;
    if (aimingFrame.getValue() == global) {
        rAim = toLocal.transformPoint(rAim);
        angle = m_trough->solveReflectionGlobal(vSunL, rAim);
    } else if (aimingFrame.getValue() == primary) {
        angle = m_trough->solveReflectionPrimary(vSunL, rAim);
    }

    // rotate nodes
    auto node = static_cast<SoBaseKit*>(parent->getPart("childList[0]", false));
    if (!node) return;
    SoTransform* tPrimary = (SoTransform*) node->getPart("transform", true);
    tPrimary->translation = primaryShift.getValue();
    tPrimary->rotation.setValue(primaryAxis.getValue(), angle);
}

void TrackerTrough::onModified(void* data, SoSensor*)
{
    TrackerTrough* tracker = (TrackerTrough*) data;
    if (tracker->m_trough) delete tracker->m_trough;

    Vector2D pa = tgf::makeVector2D(tracker->primaryAngles.getValue());

    tracker->m_trough = new TroughModel(
        TrackingDrive(
            tgf::makeVector3D(tracker->primaryShift.getValue()),
            tgf::makeVector3D(tracker->primaryAxis.getValue()),
            IntervalAngular(pa.x, pa.y)
        ),
        TrackingVertex(
            tgf::makeVector3D(tracker->facetShift.getValue()),
            tgf::makeVector3D(tracker->facetNormal.getValue())
        )
    );
}
