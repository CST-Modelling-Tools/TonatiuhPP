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

void TPerspectiveCamera::findShiftAnchor(SoQtExaminerViewer* viewer, QPoint pos, SoNode* root)
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
    vec3d d0(x, -y, -z);
    d0.normalize();

    //findAngles
    // Rcamera(gamma, alpha) d0 = d

    double ma = -m_rotationAnchor.z;
    double mb = d0.x;
    double mk = 1. - ma*ma - mb*mb;
    if (mk < 0) return;
    mk = -std::sqrt(mk);

    vec3d a(0., 0., -1.);
    vec3d b(1., 0., 0.);
//    vec3d k(0., -1., 0.);
    vec3d m(mb, -mk, -ma);

    m_rotation.x = std::atan2(triple(a, m, m_rotationAnchor), dot(m, m_rotationAnchor) - ma*ma)/gcf::degree;
    m_rotation.y = std::atan2(triple(b, d0, m), dot(m, d0) - mb*mb)/gcf::degree-90;

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
