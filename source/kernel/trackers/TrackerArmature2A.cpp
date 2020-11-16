#include "TrackerArmature2A.h"

#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/actions/SoSearchAction.h>

#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/node/TonatiuhFunctions.h"
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
    isBuiltIn = TRUE;

    SO_NODE_ADD_FIELD( primaryShift, (0.f, 0.f, 1.f) );
    SO_NODE_ADD_FIELD( primaryAxis, (0.f, 0.f, -1.f) ); // azimuth
    SO_NODE_ADD_FIELD( primaryAngles, (-90.f, 90.f) );

    SO_NODE_ADD_FIELD( secondaryShift, (0.f, 0.f, 0.f) );
    SO_NODE_ADD_FIELD( secondaryAxis, (1.f, 0.f, 0.f) ); // elevation
    SO_NODE_ADD_FIELD( secondaryAngles, (-90.f, 90.f) );

    SO_NODE_ADD_FIELD( facetShift, (0.f, 0.f, 0.f) );
    SO_NODE_ADD_FIELD( facetNormal, (0.f, 0.f, 1.f) );

    SO_NODE_ADD_FIELD( anglesDefault, (0., 0.) );

    m_solver = new TrackerSolver2A(this);

    m_sensor = new SoNodeSensor(onModified, this);
    m_sensor->setPriority(0); // important
    m_sensor->attach(this);
    onModified(this, 0);
}

TrackerArmature2A::~TrackerArmature2A()
{
    delete m_sensor;
    delete m_solver;
}


static SoTransform* findPart(SoShapeKit* parent, const char* name)
{
    // sopath does not include hidden
    // sort part during import
//    qDebug() << "be";
//    for (int q = 0; q < path->getLength(); ++q)
//        qDebug() << path->getNodeFromTail(q)->getName().getString();
//    qDebug() << "fin";

    SoSearchAction coinSearch;
    coinSearch.setInterest(SoSearchAction::FIRST);
    coinSearch.setName(name);
    coinSearch.apply(parent);
    SoFullPath* path = (SoFullPath*) coinSearch.getPath();
    if (!path) return 0;
    SoShapeKit* shape = (SoShapeKit*) path->getTail();
    return (SoTransform*) shape->getPart("transform", true);
}

#include <QDebug>
void TrackerArmature2A::update(TSeparatorKit* parent, const Transform& toGlobal,
                               const vec3d& vSun, TrackerTarget* target)
{
    QVector<Angles> solutions;
    Transform toLocal = toGlobal.inversed();
    vec3d vSunL = toLocal.transformVector(vSun);
    vec3d rAim = tgf::makeVector3D(target->aimingPoint.getValue());
    if (target->aimingType.getValue() == TrackerTarget::global) {
        rAim = toLocal.transformPoint(rAim);
        solutions = m_solver->solveReflectionGlobal(vSunL, rAim);
    } else if (target->aimingType.getValue() == TrackerTarget::local) {
        solutions = m_solver->solveReflectionSecondary(vSunL, rAim);
    }
    Angles solution = m_solver->selectSolution(solutions);
    target->angles.setValue(solution.x/gcf::degree, solution.y/gcf::degree);
}

void TrackerArmature2A::updateShape(TSeparatorKit* parent, SoShapeKit* shape, TrackerTarget* target)
{
    float alpha = target->angles.getValue()[0]*gcf::degree;
    float beta = target->angles.getValue()[1]*gcf::degree;

    // rotate nodes
    TSeparatorKit* nodePrimary = 0;
    SoGroup* childList = (SoGroup*) parent->getPart("group", false);
    for (int q = 0; q < childList->getNumChildren(); ++q) {
        nodePrimary = dynamic_cast<TSeparatorKit*>(childList->getChild(q));
        if (nodePrimary) break;
    }
    if (!nodePrimary) return;
    TTransform* tPrimary = (TTransform*) nodePrimary->getPart("transform", true);
    tPrimary->translation = primaryShift.getValue();
    tPrimary->rotation.setValue(primaryAxis.getValue(), alpha);

    TSeparatorKit* nodeSecondary = 0;
    childList = (SoGroup*) nodePrimary->getPart("group", false);
    for (int q = 0; q < childList->getNumChildren(); ++q) {
        nodeSecondary = dynamic_cast<TSeparatorKit*>(childList->getChild(q));
        if (nodeSecondary) break;
    }
    if (!nodeSecondary) return;
    TTransform* tSecondary = (TTransform*) nodeSecondary->getPart("transform", true);
    tSecondary->translation = secondaryShift.getValue();
    tSecondary->rotation.setValue(secondaryAxis.getValue(), beta);

//    SoShapeKit* shapeKit = parent;
//    SoGroup* childList = (SoGroup*) parent->getPart("group", false);
//    for (int q = 0; q < childList->getNumChildren(); ++q) {
//        shapeKit = dynamic_cast<TShapeKit*>(childList->getChild(q));
//        if (shapeKit) break;
//    }
//    if (!shapeKit) return;

//    SoNodeKitListPart* cList = (SoNodeKitListPart*) shapeKit->m_shapeKit->getPart("childList", true);
    SoTransform* st;
    SbMatrix m1, m2, m3, mP, mS;

    st = findPart(shape, "Primary");
    if (!st) return;
    m1.setTranslate(primaryShift.getValue());
    m2.setRotate(SbRotation(primaryAxis.getValue(), alpha));
    m3.setTranslate(-primaryShift.getValue());
    mP = m3*m2*m1;
    st->setMatrix(mP);

    st = findPart(shape, "Secondary");
    if (!st) return;
    m1.setTranslate( primaryShift.getValue() + secondaryShift.getValue());
    m2.setRotate(SbRotation(secondaryAxis.getValue(), beta));
    m3.setTranslate(-primaryShift.getValue() - secondaryShift.getValue());
    mS = m3*m2*m1*mP;
    st->setMatrix(mS);
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
