#include "TCameraKit.h"

SO_KIT_SOURCE(TCameraKit)

#include <Inventor/nodes/SoGroup.h>

void TCameraKit::initClass()
{
    SO_KIT_INIT_CLASS(TCameraKit, SoBaseKit, "BaseKit");
}

TCameraKit::TCameraKit()
{
    SO_KIT_CONSTRUCTOR(TCameraKit);

    SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoGroup, FALSE, this, "", FALSE);
    SO_NODE_ADD_FIELD(position, (SbVec3f(0, 0, 10))) ;
    SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
    SO_NODE_ADD_FIELD(angle, (45.) );
    SO_NODE_ADD_FIELD(perspective, (TRUE) );
    SO_KIT_INIT_INSTANCE();

    setName("Camera");

//    m_cameraKit = new SoCameraKit;
//    SoGroup* g = (SoGroup*) topSeparator.getValue();
//    g->addChild(m_cameraKit);

//    SoPerspectiveCamera* camera = (SoPerspectiveCamera*)m_cameraKit->getPart("camera", false);
//    camera->position.connectFrom(&position);
//    camera->orientation.connectFrom(&rotation);
    //    position.connectFrom(&m_camera->position);
}

//void TCameraKit::setCamera(SoCamera* camera)
//{
//    m_camera = (SoPerspectiveCamera*) camera;
//    position.connectFrom(&m_camera->position);
//    rotation.connectFrom(&m_camera->orientation);
//}
