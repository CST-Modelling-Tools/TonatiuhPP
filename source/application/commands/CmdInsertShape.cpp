#include "CmdInsertShape.h"

#include <Inventor/nodekits/SoBaseKit.h>

#include "libraries/math/gcf.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/run/InstanceNode.h"
#include "tree/SceneModel.h"

/**
 * Creates a new shapekit insert command that adds a \a shapekit node to a node given with the \a parentIndex node in the \a model.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdInsertShape::CmdInsertShape(
    TShapeKit* node,
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
        gcf::SevereError("CmdInsertShapeKit called with NULL TShapeKit*");
    m_node->ref();

    if (!parentIndex.isValid())
        gcf::SevereError("CmdInsertShapeKit called with invalid ModelIndex.");
    InstanceNode* instanceParent = m_model->getInstance(parentIndex);
    m_nodeParent = static_cast<SoBaseKit*>(instanceParent->getNode());

    setText("Insert shape");
}

/*!
 * Destroys the CmdInsertShapeKit object.
 */
CmdInsertShape::~CmdInsertShape()
{
    m_node->unref();
}

/*!
 * Reverts model state. After undo() is called, the \a shapekit node will be removed from the parent node.
 * \sa redo().
 */
void CmdInsertShape::undo()
{
    m_model->removeCoinNode(m_row, m_nodeParent);
}

/*!
 * Applies a change to the model. After redo() the model will contain new \a shapekit node.
 * \sa undo().
 */
void CmdInsertShape::redo()
{
    m_row = m_model->insertCoinNode(m_node, m_nodeParent);
}
