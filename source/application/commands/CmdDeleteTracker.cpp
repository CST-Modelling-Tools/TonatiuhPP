#include "CmdDeleteTracker.h"

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoSceneKit.h>
#include "libraries/geometry/gf.h"
#include "tree/SceneModel.h"
#include "kernel/run/InstanceNode.h"
#include "kernel/sun/TLightKit.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/trackers/TrackerAbstract.h"

/*!
 * Contructor.
 */
CmdDeleteTracker::CmdDeleteTracker(const QModelIndex& selectedIndex, SoSceneKit* scene, SceneModel& model, QUndoCommand* parent):
    QUndoCommand("Delete", parent),
    m_tracker(0),
    m_coinParent(0),
    m_scene(scene),
    m_pModel(&model),
    m_row(0)
{
    //if( !m_scene->getPart("lightList[0]", false) )     gf::SevereError( "CmdDeleteTracker Null lightKit." );

    if (!selectedIndex.isValid() ) gf::SevereError("CmdDeleteTracker called with invalid ModelIndex.");
    InstanceNode* instanceSelection = m_pModel->NodeFromIndex(selectedIndex);

    if (!instanceSelection->getNode() ) gf::SevereError("CmdDeleteTracker called with NULL selection node.");
    m_coinParent = static_cast< TSeparatorKit* > (instanceSelection->getParent()->getNode() );
    if (!m_coinParent) gf::SevereError("CmdDeleteTracker called with invalid tracker parent.");

    m_tracker = dynamic_cast<TrackerAbstract*> (m_coinParent->getPart("tracker", false) );
    if (!m_tracker) gf::SevereError("CmdDeleteTracker Null tracker.");
    m_tracker->ref();

    m_row = instanceSelection->getParent()->children.indexOf(instanceSelection);
}

/*!
 * Destructor.
 */
CmdDeleteTracker::~CmdDeleteTracker()
{
    m_tracker->unref();
}

/*!
 * Restores the tracker to the state before deletion.
 * \sa redo
 */
void CmdDeleteTracker::undo()
{
    SoTransform* parentTransform = static_cast< SoTransform* > (m_coinParent->getPart("transform", true) );
    if (!parentTransform) gf::SevereError("CmdInsertTracker Null node transform.");

//    TLightKit* lightKit = static_cast< TLightKit* >(m_scene->getPart("lightList[0]", false) );
//    if (lightKit)
//    {
//        m_tracker->SetAzimuthAngle(&lightKit->azimuth);
//        m_tracker->SetZenithAngle(&lightKit->zenith);
//    }

//    m_tracker->ConnectParentTranform(parentTransform);
    m_pModel->Paste(tgc::Shared, *m_coinParent, *m_tracker, m_row);
}

/**
 * Deletes a tracker from the scene.
 * \sa undo
 */
void CmdDeleteTracker::redo()
{
    SoTransform* transform = static_cast< SoTransform* > (m_coinParent->getPart("transform", true) );
    transform->translation.disconnect();
    transform->rotation.disconnect();
    transform->scaleFactor.disconnect();
    transform->scaleOrientation.disconnect();
    transform->center.disconnect();
    //m_tracker->Disconnect();

    m_pModel->Cut(*m_coinParent, m_row);
}
