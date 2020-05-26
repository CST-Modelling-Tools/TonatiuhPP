#include "CmdInsertTracker.h"

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoSceneKit.h>

#include "tree/SceneModel.h"
#include "libraries/geometry/gcf.h"
#include "kernel/run/InstanceNode.h"
#include "kernel/sun/SunKit.h"
#include "kernel/trackers/Tracker.h"

/**
 * Creates a new tracker insert command that adds a \a tracker to a parent node with \a parentIndex in the \a model.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdInsertTracker::CmdInsertTracker(
    Tracker* tracker,
    const QModelIndex& parentIndex,
    SceneModel* model,
    QUndoCommand* parent
):
    QUndoCommand("Insert Tracker", parent),
    m_tracker(tracker),
    m_coinParent(0),
    m_model(model),
    m_row(0)
{
    if (!m_tracker) gcf::SevereError("CmdInsertTracker Null tracker.");
    m_tracker->ref();

    if (!parentIndex.isValid() )
        gcf::SevereError("CmdInsertTracker called with invalid ModelIndex.");

    InstanceNode* instanceParent = m_model->getInstance(parentIndex);
    if (!instanceParent->getNode())
        gcf::SevereError("CmdInsertTracker called with NULL parent node.");

    m_coinParent = static_cast<SoBaseKit*>(instanceParent->getNode());
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
    m_model->removeCoinNode(m_row, *m_coinParent);
}

/**
 * Inserts the tracker in the scene model as a \a parentIndex child.
 */
void CmdInsertTracker::redo()
{
    m_row = m_model->insertCoinNode(m_tracker, m_coinParent);
}
