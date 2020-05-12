#include "CmdInsertTracker.h"

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoSceneKit.h>

#include "tree/SceneModel.h"
#include "libraries/geometry/gf.h"
#include "kernel/run/InstanceNode.h"
#include "kernel/sun/TLightKit.h"
#include "kernel/trackers/TrackerAbstract.h"

/**
 * Creates a new tracker insert command that adds a \a tracker to a parent node with \a parentIndex in the \a model.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdInsertTracker::CmdInsertTracker(TrackerAbstract* tracker,  const QModelIndex& parentIndex, SoSceneKit* scene, SceneModel* model, QUndoCommand* parent):
    QUndoCommand("Insert Tracker", parent), m_tracker (tracker), m_coinParent(0), m_scene(scene), m_pModel(model), m_row(0)
{
    if (!m_tracker) gf::SevereError("CmdInsertTracker Null tracker.");
    m_tracker->ref();

    if (!parentIndex.isValid() ) gf::SevereError("CmdInsertTracker called with invalid ModelIndex.");
    InstanceNode* instanceParent = m_pModel->NodeFromIndex(parentIndex);
    if (!instanceParent->GetNode() ) gf::SevereError("CmdInsertTracker called with NULL parent node.");
    m_coinParent = static_cast< SoBaseKit* > (instanceParent->GetNode() );
}

/*!
 * Destroys the CmdInsertTracker object.
 */
CmdInsertTracker::~CmdInsertTracker()
{
    m_tracker->unref();
}

/*!
 * Reverts model state. After undo() is called, the parentIndex node will not contain a tracker type node.
 * \sa redo().
 */
void CmdInsertTracker::undo()
{
//    m_tracker->Disconnect();
    m_pModel->RemoveCoinNode(m_row, *m_coinParent);
}

/**
 * Inserts the tracker in the scene model as a \a parentIndex child.
 */
void CmdInsertTracker::redo()
{
//    TLightKit* lightKit = static_cast< TLightKit* >(m_scene->getPart("lightList[0]", false) );
//    if (lightKit)
//    {
//        m_tracker->SetAzimuthAngle(&lightKit->azimuth);
//        m_tracker->SetZenithAngle(&lightKit->zenith);
//    }
    m_row = m_pModel->InsertCoinNode(*m_tracker, *m_coinParent);

}
