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

    void setRayPressed(SoQtExaminerViewer* viewer, QPoint pos, SoNode* root);
    void setRayMove(SoQtExaminerViewer* viewer, QPoint pos, double zoom = 0.);
    void saveTransform();
    void updateTransform();

    void shift(vec3d s);
    void rotate(double dAz, double dEl);
    void rotateAnchored(double dAz, double dEl);

protected:
    SoPerspectiveCamera* m_camera;

    vec3d m_position0;
    vec3d m_rotation0;
};

