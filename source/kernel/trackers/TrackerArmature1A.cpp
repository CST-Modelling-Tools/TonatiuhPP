#include "TrackerArmature1A.h"

#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoTransform.h>

#include "kernel/scene/TSeparatorKit.h"
#include "kernel/node/TonatiuhFunctions.h"
#include "TrackerSolver1A.h"
#include "TrackerTarget.h"

SO_NODE_SOURCE(TrackerArmature1A)


void TrackerArmature1A::initClass()
{
    SO_NODE_INIT_CLASS(TrackerArmature1A, TrackerArmature, "TrackerArmature");
}

TrackerArmature1A::TrackerArmature1A()
{
    SO_NODE_CONSTRUCTOR(TrackerArmature1A);
    isBuiltIn = TRUE;

    SO_NODE_ADD_FIELD( primaryShift, (0.f, 0.f, 1.f) );
    SO_NODE_ADD_FIELD( primaryAxis, (0.f, 1.f, 0.f) );
    SO_NODE_ADD_FIELD( primaryAngles, (-90.f, 90.f) );

    SO_NODE_ADD_FIELD( facetShift, (0.f, 0.f, 0.f) );
    SO_NODE_ADD_FIELD( facetNormal, (0.f, 0.f, 1.f) );

    SO_NODE_ADD_FIELD( angleDefault, (0.f) );

    m_solver = new TrackerSolver1A(this);

    m_sensor = new SoNodeSensor(onModified, this);
    m_sensor->setPriority(0);
    m_sensor->attach(this);
    onModified(this, 0);
}

TrackerArmature1A::~TrackerArmature1A()
{
    delete m_sensor;
    delete m_solver;
}

void TrackerArmature1A::update(TSeparatorKit* /*parent*/, const Transform& toGlobal,
                               const vec3d& vSun, TrackerTarget* target)
{
    Transform toLocal = toGlobal.inversed();
    vec3d vSunL = toLocal.transformVector(vSun);
    vec3d rAim = tgf::makeVector3D(target->aimingPoint.getValue());

    double angle;
    if (target->aimingType.getValue() == TrackerTarget::global) {
        rAim = toLocal.transformPoint(rAim);
        angle = m_solver->solveReflectionGlobal(vSunL, rAim);
    } else if (target->aimingType.getValue() == TrackerTarget::local) {
        angle = m_solver->solveReflectionPrimary(vSunL, rAim);
    } else {
        angle = 0;
    }
    angle = m_solver->selectSolution(angle);
    target->angles.setValue(angle/gcf::degree, 0.);
}

void TrackerArmature1A::updateShape(TSeparatorKit* parent, SoShapeKit* /*shape*/, TrackerTarget* target)
{
    float alpha = target->angles.getValue()[0]*gcf::degree;

    // rotate nodes
    TSeparatorKit* node = 0;
    SoGroup* childList = (SoGroup*) parent->getPart("group", false);
    for (int q = 0; q < childList->getNumChildren(); ++q) {
        node = dynamic_cast<TSeparatorKit*>(childList->getChild(q));
        if (node) break;
    }
    if (!node) return;
    TTransform* tPrimary = (TTransform*) node->getPart("transform", true);
    tPrimary->translation = primaryShift.getValue();
    tPrimary->rotation.setValue(primaryAxis.getValue(), alpha);
}

void TrackerArmature1A::onModified(void* data, SoSensor*)
{
    TrackerArmature1A* tracker = (TrackerArmature1A*) data;
    tracker->onModified();
}

void TrackerArmature1A::onModified()
{
    vec2d pa = tgf::makeVector2D(primaryAngles.getValue())*gcf::degree;
    primary = ArmatureJoint(
        tgf::makeVector3D(primaryShift.getValue()),
        tgf::makeVector3D(primaryAxis.getValue()),
        IntervalPeriodic(pa.x, pa.y)
    );

    facet = ArmatureVertex(
        tgf::makeVector3D(facetShift.getValue()),
        tgf::makeVector3D(facetNormal.getValue())
    );

    facet.normal -= dot(facet.normal, primary.axis)*primary.axis;
    facet.normal.normalize();

    angle0 = angleDefault.getValue();
}
