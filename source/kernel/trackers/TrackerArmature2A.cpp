#include "TrackerArmature2A.h"

#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodes/SoGroup.h>

#include "kernel/scene/TSeparatorKit.h"
#include "kernel/trf.h"
#include "TrackerSolver2A.h"
#include "TrackerTarget.h"

SO_NODE_SOURCE(TrackerArmature2A)


void TrackerArmature2A::initClass()
{
    SO_NODE_INIT_CLASS(TrackerArmature2A, TrackerArmature, "TrackerArmature");
}

TrackerArmature2A::TrackerArmature2A()
{
    SO_NODE_CONSTRUCTOR(TrackerArmature2A);

    SO_NODE_ADD_FIELD( primaryShift, (0.f, 0.f, 1.f) );
    SO_NODE_ADD_FIELD( primaryAxis, (0.f, 0.f, -1.f) ); // azimuth
    SO_NODE_ADD_FIELD( primaryAngles, (-90.f, 90.f) );

    SO_NODE_ADD_FIELD( secondaryShift, (0.f, 0.f, 0.1f) );
    SO_NODE_ADD_FIELD( secondaryAxis, (1.f, 0.f, 0.f) ); // elevation
    SO_NODE_ADD_FIELD( secondaryAngles, (-90.f, 90.f) );

    SO_NODE_ADD_FIELD( facetShift, (0.f, 0.f, 0.f) );
    SO_NODE_ADD_FIELD( facetNormal, (0.f, 0.f, 1.f) );

    SO_NODE_ADD_FIELD( anglesDefault, (0., 0.) );

    m_solver = new TrackerSolver2A(this);

    m_sensor = new SoNodeSensor(onModified, this);
    m_sensor->setPriority(0);
    m_sensor->attach(this);
    onModified(this, 0);
}

TrackerArmature2A::~TrackerArmature2A()
{
    delete m_sensor;
    delete m_solver;
}

void TrackerArmature2A::update(TSeparatorKit* parent, const Transform& toGlobal,
                               const vec3d& vSun, TrackerTarget* target)
{
    QVector<Angles> solutions;
    Transform toLocal = toGlobal.inversed();
    vec3d vSunL = toLocal.transformVector(vSun);
    vec3d rAim = tgf::makeVector3D(target->aimingPoint.getValue());
    if (target->aimingFrame.getValue() == TrackerTarget::global) {
        rAim = toLocal.transformPoint(rAim);
        solutions = m_solver->solveReflectionGlobal(vSunL, rAim);
    } else if (target->aimingFrame.getValue() == TrackerTarget::facets) {
        solutions = m_solver->solveReflectionSecondary(vSunL, rAim);
    }
    Angles solution = m_solver->selectSolution(solutions);

    // rotate nodes
    SoGroup* childList = (SoGroup*) parent->getPart("group", false);
    for (int q = 0; q < childList->getNumChildren(); ++q) {
        TSeparatorKit* nodePrimary = dynamic_cast<TSeparatorKit*>(childList->getChild(q));
        if (!nodePrimary) continue;
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
        break;
    }
}

void TrackerArmature2A::onModified(void* data, SoSensor*)
{
    TrackerArmature2A* tracker = (TrackerArmature2A*) data;
    tracker->onModified();
}

void TrackerArmature2A::onModified()
{
    vec2d pa = tgf::makeVector2D(primaryAngles.getValue())*gcf::degree;
    primary = ArmatureJoint(
        tgf::makeVector3D(primaryShift.getValue()),
        tgf::makeVector3D(primaryAxis.getValue()),
        IntervalPeriodic(pa.x, pa.y)
    );

    vec2d pb = tgf::makeVector2D(secondaryAngles.getValue())*gcf::degree;
    secondary = ArmatureJoint(
        tgf::makeVector3D(secondaryShift.getValue()),
        tgf::makeVector3D(secondaryAxis.getValue()),
        IntervalPeriodic(pb.x, pb.y)
    );

    facet = ArmatureVertex(
        tgf::makeVector3D(facetShift.getValue()),
        tgf::makeVector3D(facetNormal.getValue())
    );

    angles0 = tgf::makeVector2D(anglesDefault.getValue());
}
