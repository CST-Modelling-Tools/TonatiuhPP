#pragma once

#include <Inventor/nodes/SoPerspectiveCamera.h>
#include "libraries/math/3D/vec3d.h"
#include <QPoint>
class SoQtExaminerViewer;
class TSeparatorKit;

class TPerspectiveCamera
{
public:
    TPerspectiveCamera();

    SoPerspectiveCamera* camera() const {return m_camera;}
    void setCamera(SoPerspectiveCamera* camera);

    vec3d m_position;
    vec3d m_rotation; // {azimuth, elevation, 0}
    vec3d m_anchor;
    bool m_isAnchored;
    vec3d m_rotationAnchor; // direction

    vec3d m_orbitAnchor;
    bool m_isOrbitAnchored;
    bool m_useNear;

    void findMoveAnchor(SoQtExaminerViewer* viewer, QPoint pos, SoNode* root);
    void moveShiftAnchor(SoQtExaminerViewer* viewer, QPoint pos, double zoom = 0.);
    void movePanAnchor(SoQtExaminerViewer* viewer, QPoint pos, double zoom = 0.);

    void findRotationAnchor(SoQtExaminerViewer* viewer, QPoint pos);
    void moveRotationAnchor(SoQtExaminerViewer* viewer, QPoint pos);

    void findOrbitAnchor(SoQtExaminerViewer* viewer, QPoint pos, SoNode* root);
    void moveOrbitAnchor(SoQtExaminerViewer* viewer, QPoint pos);

    void zoomCenter(SoQtExaminerViewer* viewer, SoNode* root, double zoom = 0.);
    void moveCameraPlane(SoQtExaminerViewer* viewer, QPoint pos, SoNode* root, double zoom);

    void saveTransform();
    void updateTransform();

    void shift(vec3d s);
    void rotate(double dAz, double dEl);
    void orbit(double dAz, double dEl);
    void setRotation(double az, double el);
    void lookAt(vec3d aim);
//    void rotateAnchored(double dAz, double dEl);

protected:
    SoPerspectiveCamera* m_camera;

    vec3d m_position0;
    vec3d m_rotation0;
};

