#include "TrackerHeliostat.h"

#include <Inventor/sensors/SoNodeSensor.h>

#include "kernel/trf.h"
#include "HeliostatModel.h"

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
    m_sensor->attach(this);
    onModified(this, 0);
}

TrackerHeliostat::~TrackerHeliostat()
{
    delete m_sensor;
    delete m_heliostat;
}

void TrackerHeliostat::update(SoBaseKit* parent, const Transform& toGlobal, const Vector3D& vSun)
{
    QVector<Angles> solutions;
    Transform toLocal = toGlobal.inversed();
    Vector3D vSunL = toLocal.transformVector(vSun);
    Vector3D rAim = tgf::makeVector3D(aimingPoint.getValue());
    if (aimingFrame.getValue() == global) {
        rAim = toLocal.transformPoint(rAim);
        solutions = m_heliostat->solveReflectionGlobal(vSunL, rAim);
    } else if (aimingFrame.getValue() == secondary) {
        solutions = m_heliostat->solveReflectionSecondary(vSunL, rAim);
    }
    Angles solution = m_heliostat->selectSolution(solutions);

    // rotate nodes
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

void TrackerHeliostat::onModified(void* data, SoSensor*)
{
    TrackerHeliostat* tracker = (TrackerHeliostat*) data;
    if (tracker->m_heliostat) delete tracker->m_heliostat;

    Vector2D pa = tgf::makeVector2D(tracker->primaryAngles.getValue());
    Vector2D pb = tgf::makeVector2D(tracker->secondaryAngles.getValue());

    tracker->m_heliostat = new HeliostatModel(
        TrackingDrive(
            tgf::makeVector3D(tracker->primaryShift.getValue()),
            tgf::makeVector3D(tracker->primaryAxis.getValue()),
            IntervalAngular(pa.x, pa.y)
        ),
        TrackingDrive(
            tgf::makeVector3D(tracker->secondaryShift.getValue()),
            tgf::makeVector3D(tracker->secondaryAxis.getValue()),
            IntervalAngular(pb.x, pb.y)
        ),
        TrackingVertex(
            tgf::makeVector3D(tracker->facetShift.getValue()),
            tgf::makeVector3D(tracker->facetNormal.getValue())
        ),
        tgf::makeVector2D(tracker->anglesDefault.getValue())
    );
}
