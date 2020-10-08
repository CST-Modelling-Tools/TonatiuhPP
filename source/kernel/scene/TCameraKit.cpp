#include "TCameraKit.h"

SO_KIT_SOURCE(TCameraKit)

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include "kernel/scene/TPerspectiveCamera.h"


void TCameraKit::initClass()
{
    SO_KIT_INIT_CLASS(TCameraKit, SoBaseKit, "BaseKit");
}

TCameraKit::TCameraKit()
{
//    m_sensor_pos = new SoFieldSensor(updatePR, this);
//    m_sensor->setPriority(0);
//    m_sensor_rot = new SoFieldSensor(updatePR, this);
//    m_sensor_rot->setPriority(0);


    SO_KIT_CONSTRUCTOR(TCameraKit);

    SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoGroup, FALSE, this, "", FALSE);
    SO_NODE_ADD_FIELD(position, (SbVec3f(0, 0, 10)));
    SO_NODE_ADD_FIELD(rotation, (SbVec2f(0.0f, 0.0f)));
    SO_NODE_ADD_FIELD(angle, (30.) );
    SO_NODE_ADD_FIELD(perspective, (TRUE) );
    SO_KIT_INIT_INSTANCE();

    setName("Camera");

    m_sensor = new SoNodeSensor(update, this);
    m_sensorPR = new SoNodeSensor(updatePR, this);
    m_sensorPR->attach(this);
//    m_sensor_rot->attach(&rotation);

//    m_cameraKit = new SoCameraKit;
//    SoGroup* g = (SoGroup*) topSeparator.getValue();
//    g->addChild(m_cameraKit);

//    SoPerspectiveCamera* camera = (SoPerspectiveCamera*)m_cameraKit->getPart("camera", false);
//    camera->position.connectFrom(&position);
//    camera->orientation.connectFrom(&rotation);
    //    position.connectFrom(&m_camera->position);
    m_camera = 0;

}

TCameraKit::~TCameraKit()
{
    delete m_sensor;
}

void TCameraKit::setCamera(TPerspectiveCamera* camera)
{
    m_camera = camera;
    m_sensor->attach(m_camera->camera());
    syncPR();
}

void TCameraKit::update(void* data, SoSensor*)
{
    TCameraKit* kit = (TCameraKit*) data;
    kit->sync();
}

void TCameraKit::updatePR(void* data, SoSensor*)
{
    TCameraKit* kit = (TCameraKit*) data;
    kit->syncPR();
}

void TCameraKit::sync()
{
    enableNotify(FALSE);
    vec3d& p = m_camera->m_position;
    position.setValue(p[0], p[1], p[2]);
    vec3d& r = m_camera->m_rotation;
    rotation.setValue(r[0], r[1]);
    enableNotify(TRUE);
}

#include "kernel/node/TonatiuhFunctions.h"
void TCameraKit::syncPR()
{
    if (!m_camera) return;
    m_camera->camera()->enableNotify(FALSE);
    m_camera->m_position = tgf::makeVector3D(position.getValue());
    m_camera->m_rotation = tgf::makeVector2D(rotation.getValue());
    m_camera->updateTransform();
   m_camera->camera()->enableNotify(TRUE);
}

//void TCameraKit::setCamera(SoCamera* camera)
//{
//    m_camera = (SoPerspectiveCamera*) camera;
//    position.connectFrom(&m_camera->position);
//    rotation.connectFrom(&m_camera->orientation);
//}
