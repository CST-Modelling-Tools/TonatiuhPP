#include "CmdDelete.h"

#include <Inventor/nodekits/SoBaseKit.h>
#include "kernel/run/InstanceNode.h"
#include "tree/SceneModel.h"

/**
 * Creates a new delete command that represents the \a selectedIndex node in the \a model deletion.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */

CmdDelete::CmdDelete(const QModelIndex& index, SceneModel* model, QUndoCommand* parent):
    QUndoCommand("Delete", parent),
    m_node(0),
    m_nodeParent(0),
    m_model(model),
    m_row(-1)
{
    InstanceNode* instance = m_model->getInstance(index);
    m_node = instance->getNode();
    m_node->ref();
    InstanceNode* instanceParent = instance->getParent();
    m_nodeParent = static_cast<SoBaseKit*>(instanceParent->getNode());
    m_row = instanceParent->children.indexOf(instance);
}

CmdDelete::~CmdDelete()
{
    m_node->unref();
}

/*!
 * Reverts deletion. After undo() is called, the state of the model will be the same as before redo() was called.
 * \sa redo().
 */
void CmdDelete::undo()
{
    m_model->Paste(tgc::Shared, *m_nodeParent, *m_node, m_row);
}

/*!
 * Applies a change to the scene. After redo() the scene will not contain the node located on the model as selectedIndex.
 * \sa undo().
 */
void CmdDelete::redo()
{
    m_model->Cut(*m_nodeParent, m_row);
}
