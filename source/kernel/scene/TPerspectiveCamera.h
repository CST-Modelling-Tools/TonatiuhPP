#pragma once
#include "kernel/TonatiuhKernel.h"

#include <Inventor/nodes/SoPerspectiveCamera.h>
#include "libraries/math/3D/vec3d.h"
#include <QPoint>
class SoQtExaminerViewer;
class TSeparatorKit;

class TONATIUH_KERNEL TPerspectiveCamera
{
public:
    TPerspectiveCamera();

    SoPerspectiveCamera* camera() const {return m_camera;}
    void setCamera(SoPerspectiveCamera* camera);

    vec3d m_position;
    vec3d m_rotation; // {azimuth, elevation, 0}

    void saveTransform();
    void updateTransform();

    void shift(vec3d s);
    void rotate(double dAz, double dEl);
    void setRotation(double az, double el);
    void setRotationAt(vec3d aim);
    void orbit(double dAz, double dEl);
    void setOrbit(double az, double el, vec3d anchor);

    vec3d findRayGlobal0();
    vec3d findRayGlobal(SoQtExaminerViewer* viewer, QPoint pos);
    vec3d findRayLocal(SoQtExaminerViewer* viewer, QPoint pos);
    bool findAnchor(SoQtExaminerViewer* viewer, QPoint pos, SoNode* root, vec3d& anchor);
    bool findAnchor0(SoQtExaminerViewer* viewer, SoNode* root, vec3d& anchor);

    void findMoveAnchor(SoQtExaminerViewer* viewer, QPoint pos, SoNode* root);
    void moveShiftAnchor(SoQtExaminerViewer* viewer, QPoint pos, double zoom = 0.);
    void movePanAnchor(SoQtExaminerViewer* viewer, QPoint pos, double zoom = 0.);

    void findRotationAnchor(SoQtExaminerViewer* viewer, QPoint pos);
    void moveRotationAnchor(SoQtExaminerViewer* viewer, QPoint pos);

    void findOrbitAnchor(SoQtExaminerViewer* viewer, QPoint pos, SoNode* root);
    void moveOrbitAnchor(SoQtExaminerViewer* viewer, QPoint pos);

    void zoomCenter(SoQtExaminerViewer* viewer, SoNode* root, double zoom = 0.);
    void moveCameraPlane(SoQtExaminerViewer* viewer, QPoint pos, SoNode* root, double zoom);

protected:
    SoPerspectiveCamera* m_camera;

    vec3d m_position0;
    vec3d m_rotation0;

    vec3d m_anchor;
    bool m_isAnchored;

    vec3d m_rotationAnchor; // direction

    vec3d m_orbitAnchor;
    bool m_isOrbitAnchored;
    bool m_useNear;
};

