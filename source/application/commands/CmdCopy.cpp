#include "CmdCopy.h"

#include "tree/SceneTreeModel.h"
#include "kernel/run/InstanceNode.h"


CmdCopy::CmdCopy(const QModelIndex& index, SoNode*& clipboard, SceneTreeModel* model, QUndoCommand* parent):
    QUndoCommand("Copy", parent),
    m_clipboard(clipboard),
    m_model(model)
{
    m_nodeOld = clipboard;
    InstanceNode* instance = m_model->getInstance(index);
    m_node = instance->getNode();
}

void CmdCopy::undo()
{
    m_clipboard = m_nodeOld;
}

void CmdCopy::redo()
{
    m_clipboard = m_node;
}
