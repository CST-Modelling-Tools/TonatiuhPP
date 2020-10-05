#include "TPerspectiveCamera.h"

#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

#include "libraries/math/gcf.h"
#include <QDebug>
#include <Inventor/SoPickedPoint.h>
#include "libraries/math/3D/Transform.h"
#include "kernel/node/TonatiuhFunctions.h"
//#include "GraphicRoot.h"
#include "kernel/scene/TSeparatorKit.h"

TPerspectiveCamera::TPerspectiveCamera()
{

}

void TPerspectiveCamera::setCamera(SoPerspectiveCamera* camera)
{
    m_camera = camera;
    m_position = vec3d(0., 0., 1.);
    m_rotation = vec3d(0., 0., 0.);
    updateTransform();
}

void TPerspectiveCamera::findMoveAnchor(SoQtExaminerViewer* viewer, QPoint pos, SoNode* root)
{
    // find ray
    SbVec2s vs = viewer->getViewportRegion().getViewportSizePixels();
    double x = pos.x()/double(vs[0])*2 - 1;
    double y = pos.y()/double(vs[1])*2 - 1;
    if (vs[0] > vs[1])
        x *= double(vs[0])/vs[1];
    else
        y *= double(vs[1])/vs[0];

    double z = 1./std::tan(m_camera->heightAngle.getValue()/2);
    vec3d rd(x, -y, -z);
    rd.normalize();
    rd = Transform::rotateX((m_rotation.y + 90.)*gcf::degree).transformVector(rd);
    rd = Transform::rotateZ(-m_rotation.x*gcf::degree).transformVector(rd);

    // pick object
    SoRayPickAction rpa(viewer->getViewportRegion());
//    rpa.setPoint(SbVec2s(pos.x(), pos.y())); // does not work
//    rpa.setRadius(3.);
    rpa.setRay(SbVec3f(m_position0.x, m_position0.y, m_position0.z), SbVec3f(rd.x, rd.y, rd.z));
    rpa.apply(root);

    const SoPickedPoint* picked = rpa.getPickedPoint();
    if (picked) {
        m_anchor = tgf::makeVector3D(picked->getPoint());
        m_isAnchored = true;
//        qDebug() << "anch " << m_anchor;
    } else {
        // horizon ro + trd = 0
        double t = -m_position0.z/rd.z;
        if (t > 0) {
            m_anchor = m_position0 + t*rd;
            m_isAnchored = true;
        } else
            m_isAnchored = false;
    }
}

void TPerspectiveCamera::moveShiftAnchor(SoQtExaminerViewer* viewer, QPoint pos, double zoom)
{
    if (!m_isAnchored) return;

    SbVec2s vs = viewer->getViewportRegion().getViewportSizePixels();
    double x = pos.x()/double(vs[0])*2 - 1;
    double y = pos.y()/double(vs[1])*2 - 1;
    if (vs[0] > vs[1])
        x *= double(vs[0])/vs[1];
    else
        y *= double(vs[1])/vs[0];
    double z = 1./std::tan(m_camera->heightAngle.getValue()/2);
    vec3d rd(x, -y, -z);
    rd.normalize();
    rd = Transform::rotateX((m_rotation.y + 90.)*gcf::degree).transformVector(rd);
    rd = Transform::rotateZ(-m_rotation.x*gcf::degree).transformVector(rd);

    // horizon ro + trd = m_anchor
    double t = (m_anchor.z - m_position0.z)/rd.z;
    t *= std::pow(0.7, -zoom);
    m_position = m_anchor - t*rd;
    m_position0.z = m_position.z;

    updateTransform();
}

void TPerspectiveCamera::findRotationAnchor(SoQtExaminerViewer* viewer, QPoint pos)
{
    // find ray
    SbVec2s vs = viewer->getViewportRegion().getViewportSizePixels();
    double x = pos.x()/double(vs[0])*2 - 1;
    double y = pos.y()/double(vs[1])*2 - 1;
    if (vs[0] > vs[1])
        x *= double(vs[0])/vs[1];
    else
        y *= double(vs[1])/vs[0];

    double z = 1./std::tan(m_camera->heightAngle.getValue()/2);
    vec3d rd(x, -y, -z);
    rd.normalize();
    rd = Transform::rotateX((m_rotation.y + 90.)*gcf::degree).transformVector(rd);
    rd = Transform::rotateZ(-m_rotation.x*gcf::degree).transformVector(rd);

    m_rotationAnchor = rd;
}

void TPerspectiveCamera::moveRotationAnchor(SoQtExaminerViewer* viewer, QPoint pos)
{
    // find ray
    SbVec2s vs = viewer->getViewportRegion().getViewportSizePixels();
    double x = pos.x()/double(vs[0])*2 - 1;
    double y = pos.y()/double(vs[1])*2 - 1;
    if (vs[0] > vs[1])
        x *= double(vs[0])/vs[1];
    else
        y *= double(vs[1])/vs[0];

    double z = 1./std::tan(m_camera->heightAngle.getValue()/2);
    vec3d d0(x, z, -y);
    d0.normalize();

    // find angles
    // Rcamera(gamma, alpha) d0 = d

    double ma = -m_rotationAnchor.z;
    double mb = d0.x;
    double mk = 1. - ma*ma - mb*mb;
    if (mk < 0) return;
    mk = std::sqrt(mk);

//    if (m_rotationAnchor.y < 0) mk = -mk; // smaller rotations?

    vec3d a(0., 0., -1.);
    vec3d b(1., 0., 0.);
//    vec3d k(0., -1., 0.);
    vec3d m(mb, mk, -ma); // y > 0 half-plane, smaller alpha

    m_rotation.x = std::atan2(triple(a, m, m_rotationAnchor), dot(m, m_rotationAnchor) - ma*ma)/gcf::degree;
    m_rotation.y = std::atan2(triple(b, d0, m), dot(m, d0) - mb*mb)/gcf::degree;

    updateTransform();
}

void TPerspectiveCamera::findOrbitAnchor(SoQtExaminerViewer* viewer, QPoint pos, SoNode* root)
{
    // find ray
    SbVec2s vs = viewer->getViewportRegion().getViewportSizePixels();
    double x = pos.x()/double(vs[0])*2 - 1;
    double y = pos.y()/double(vs[1])*2 - 1;
    if (vs[0] > vs[1])
        x *= double(vs[0])/vs[1];
    else
        y *= double(vs[1])/vs[0];

    double z = 1./std::tan(m_camera->heightAngle.getValue()/2);
    vec3d rd(x, -y, -z);
    rd.normalize();
    rd = Transform::rotateX((m_rotation.y + 90.)*gcf::degree).transformVector(rd);
    rd = Transform::rotateZ(-m_rotation.x*gcf::degree).transformVector(rd);

    // pick object
    SoRayPickAction rpa(viewer->getViewportRegion());
    rpa.setRay(SbVec3f(m_position0.x, m_position0.y, m_position0.z), SbVec3f(rd.x, rd.y, rd.z));
    rpa.apply(root);

    const SoPickedPoint* picked = rpa.getPickedPoint();
    if (picked) {
        m_orbitAnchor = tgf::makeVector3D(picked->getPoint());
        m_isOrbitAnchored = true;
    } else {
        // horizon ro + trd = 0
        double t = -m_position0.z/rd.z;
        if (t > 0) {
            m_orbitAnchor = m_position0 + t*rd;
            m_isOrbitAnchored = true;
        } else
            m_isOrbitAnchored = false;
    }

    // near or far
    double gamma = m_rotation0.x*gcf::degree;
    double alpha = m_rotation0.y*gcf::degree;
    vec3d direction(
        cos(alpha)*sin(gamma),
        cos(alpha)*cos(gamma),
        sin(alpha)
    );
    m_useNear = dot(m_anchor - m_orbitAnchor, direction) > 0;
}

#include "libraries/math/gcf.h"
void TPerspectiveCamera::moveOrbitAnchor(SoQtExaminerViewer* viewer, QPoint pos)
{
    if (!m_isAnchored) return;
    if (!m_isOrbitAnchored) return;

    // find ray
    SbVec2s vs = viewer->getViewportRegion().getViewportSizePixels();
    double x = pos.x()/double(vs[0])*2 - 1;
    double y = pos.y()/double(vs[1])*2 - 1;
    if (vs[0] > vs[1])
        x *= double(vs[0])/vs[1];
    else
        y *= double(vs[1])/vs[0];

    double z = 1./std::tan(m_camera->heightAngle.getValue()/2);
    vec3d d0(x, z, -y);
    d0.normalize();

    // find angles
    // Rcamera(gamma, alpha) {d0 + p} = rMouse - rAnchor
    vec3d rMA = m_orbitAnchor - m_anchor;
    double distance = (m_position0 - m_anchor).norm();
    vec3d p(0, distance, 0);

    double t1, t2;
    if (!gcf::solveQuadratic(1., -2.*dot(d0, p), p.norm2() - rMA.norm2(), &t1, &t2))
        return;

    if (m_useNear)
        d0 = t1*d0 - p;
    else
        d0 = t2*d0 - p;

    d0.normalize();
    rMA.normalize();

    // angles
    double ma = -rMA.z;
    double mb = d0.x;
    double mk = 1. - ma*ma - mb*mb;
    if (mk < 0) return;
    mk = std::sqrt(mk); // t1*d0 - p is mostly -y axis

     if (m_useNear)
         mk = -mk;

    vec3d a(0., 0., -1.);
    vec3d b(1., 0., 0.);
//    vec3d k(0., -1., 0.);
    vec3d m(mb, mk, -ma); // y > 0 half-plane, smaller alpha

    m_rotation.x = std::atan2(triple(a, m, rMA), dot(m, rMA) - ma*ma)/gcf::degree;
    m_rotation.y = std::atan2(triple(b, d0, m), dot(m, d0) - mb*mb)/gcf::degree;

    double gamma = m_rotation.x*gcf::degree;
    double alpha = m_rotation.y*gcf::degree;
    vec3d direction(
        cos(alpha)*sin(gamma),
        cos(alpha)*cos(gamma),
        sin(alpha)
    );
    m_position = m_anchor - distance*direction;

    updateTransform();
}

void TPerspectiveCamera::saveTransform()
{
    m_position0 = m_position;
    m_rotation0 = m_rotation;
}

void TPerspectiveCamera::updateTransform()
{
    m_camera->position.setValue(m_position.x, m_position.y, m_position.z);
    SbRotation rx(SbVec3f(1, 0, 0), (m_rotation.y + 90.)*gcf::degree);
    SbRotation rz(SbVec3f(0, 0, 1), -m_rotation.x*gcf::degree);
    m_camera->orientation = rx*rz;
}

void TPerspectiveCamera::shift(vec3d s)
{
    m_position = m_position0 + s;
    updateTransform();
}

void TPerspectiveCamera::rotate(double dAz, double dEl)
{
    m_rotation.x = m_rotation0.x + dAz;
    m_rotation.y = gcf::clamp(m_rotation0.y + dEl, -89.9, 89.9);
    updateTransform();
}

void TPerspectiveCamera::orbit(double dAz, double dEl)
{
    if (!m_isAnchored) return;

    m_rotation.x = m_rotation0.x + dAz;
    m_rotation.y = gcf::clamp(m_rotation0.y + dEl, -89.9, 89.9);

    double distance = (m_position0 - m_anchor).norm();
    double gamma = m_rotation.x*gcf::degree;
    double alpha = m_rotation.y*gcf::degree;
    vec3d direction(
        cos(alpha)*sin(gamma),
        cos(alpha)*cos(gamma),
        sin(alpha)
    );
    m_position = m_anchor - distance*direction;

    updateTransform();
}
