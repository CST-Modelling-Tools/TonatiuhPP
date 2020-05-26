#include "CmdInsertSeparatorKit.h"

#include <Inventor/nodekits/SoBaseKit.h>

#include "libraries/geometry/gcf.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/run/InstanceNode.h"
#include "tree/SceneModel.h"

/**
 * Creates a new group node insert command that adds a \a separatorKit to \a parentIndex node in the \a model.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdInsertSeparatorKit::CmdInsertSeparatorKit(
    TSeparatorKit* node,
    const QModelIndex& parentIndex,
    SceneModel* model,
    QUndoCommand* parent
):
    QUndoCommand(parent),
    m_node(node),
    m_nodeParent(0),
    m_model(model),
    m_row(-1)
{
    if (!m_node)
        gcf::SevereError("CmdInsertSeparatorKit Null separatorKit.");
    m_node->ref();

    if (!parentIndex.isValid() )
        gcf::SevereError("CmdInsertSeparatorKit called with invalid ModelIndex.");
    InstanceNode* instanceParent = m_model->getInstance(parentIndex);
    m_nodeParent = static_cast<SoBaseKit*>(instanceParent->getNode());

    setText("Insert node");
}

/*!
 * Destroys the CmdInsertSeparatorKit object.
 */
CmdInsertSeparatorKit::~CmdInsertSeparatorKit()
{
    m_node->unref();
}

/*!
 * Reverts model state. After undo() is called, the node with the parentIndex will not contain a separatorKit node.
 * \sa redo().
 */
void CmdInsertSeparatorKit::undo()
{
    m_model->removeCoinNode(m_row, *m_nodeParent);
}

/*!
 * Applies a change to the model. After redo() the model will contain new group node.
 * \sa undo().
 */
void CmdInsertSeparatorKit::redo()
{
    m_row = m_model->insertCoinNode(m_node, m_nodeParent);
}
