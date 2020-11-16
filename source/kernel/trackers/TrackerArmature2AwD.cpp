#include "TrackerArmature2AwD.h"

#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/actions/SoSearchAction.h>

#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/node/TonatiuhFunctions.h"
#include "TrackerSolver2A.h"
#include "TrackerTarget.h"

SO_NODE_SOURCE(TrackerArmature2AwD)


void TrackerArmature2AwD::initClass()
{
    SO_NODE_INIT_CLASS(TrackerArmature2AwD, TrackerArmature2A, "TrackerArmature2A");
}

TrackerArmature2AwD::TrackerArmature2AwD()
{
    SO_NODE_CONSTRUCTOR(TrackerArmature2AwD);
    isBuiltIn = TRUE;

    SO_NODE_ADD_FIELD( drivePrimaryR, (0.f, 0.f, 1.f) );
    SO_NODE_ADD_FIELD( drivePrimaryP, (0.f, 0.1f, 1.f) );

    SO_NODE_ADD_FIELD( driveSecondaryP, (0.f, 0.2f, 1.f) );
    SO_NODE_ADD_FIELD( driveSecondaryS, (0.f, 0.3f, 1.f) );
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

// rotation around a from m to v
static double findAngle(const SbVec3f& a, SbVec3f m, SbVec3f v)
{
    m.normalize();
    v.normalize();
    return atan2(a.dot(m.cross(v)), m.dot(v));
}

void TrackerArmature2AwD::update(TSeparatorKit* parent, const Transform& toGlobal, const vec3d& vSun, TrackerTarget* target)
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

void TrackerArmature2AwD::updateShape(TSeparatorKit* parent, SoShapeKit* shape, TrackerTarget* target)
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
    SoShapeKit* pShape = shape;
    SoTransform* st;
    SbMatrix m1, m2, m3, mP, mS;

    st = findPart(pShape, "Primary");
    if (!st) return;
    m1.setTranslate(primaryShift.getValue());
    m2.setRotate(SbRotation(primaryAxis.getValue(), alpha));
    m3.setTranslate(-primaryShift.getValue());
    mP = m3*m2*m1;
    st->setMatrix(mP);

    st = findPart(pShape, "Secondary");
    if (!st) return;
    m1.setTranslate( primaryShift.getValue() + secondaryShift.getValue());
    m2.setRotate(SbRotation(secondaryAxis.getValue(), beta));
    m3.setTranslate(-primaryShift.getValue() - secondaryShift.getValue());
    mS = m3*m2*m1;
    st->setMatrix(mS*mP);

    /* refactor */
    SbMatrix mD;

    SbVec3f dPR = drivePrimaryR.getValue();
    SbVec3f dPP0 = drivePrimaryP.getValue();
    SbVec3f dPP;
    mP.multVecMatrix(dPP0, dPP);
    float phi = findAngle(primaryAxis.getValue(), dPR - dPP0, dPR - dPP);

    st = findPart(pShape, "DrivePrimaryR");
    if (!st) return;
    m1.setTranslate(dPR);
    m2.setRotate(SbRotation(primaryAxis.getValue(), phi));
    m3.setTranslate(-dPR);
    mD = m3*m2*m1;
    st->setMatrix(mD);

    st = findPart(pShape, "DrivePrimaryP");
    if (!st) return;
    m1.setTranslate(dPP);
    m2.setRotate(SbRotation(primaryAxis.getValue(), phi));
    m3.setTranslate(-dPP0);
    mD = m3*m2*m1;
    st->setMatrix(mD);


    SbVec3f dSP = driveSecondaryP.getValue();
    SbVec3f dSS0 = driveSecondaryS.getValue();
    SbVec3f dSS;
    mS.multVecMatrix(dSS0, dSS);
    phi = findAngle(secondaryAxis.getValue(), dSP - dSS0, dSP - dSS);

    st = findPart(pShape, "DriveSecondaryP");
    if (!st) return;
    m1.setTranslate(dSP);
    m2.setRotate(SbRotation(secondaryAxis.getValue(), phi));
    m3.setTranslate(-dSP);
    mD = m3*m2*m1;
    st->setMatrix(mD*mP);

    st = findPart(pShape, "DriveSecondaryS");
    if (!st) return;
    m1.setTranslate(dSS);
    m2.setRotate(SbRotation(secondaryAxis.getValue(), phi));
    m3.setTranslate(-dSS0);
    mD = m3*m2*m1;
    st->setMatrix(mD*mP);
}
