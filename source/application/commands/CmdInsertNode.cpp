#include "CmdInsertNode.h"

#include <Inventor/nodekits/SoBaseKit.h>

#include "libraries/math/gcf.h"
#include "kernel/run/InstanceNode.h"
#include "tree/SceneModel.h"


CmdInsertNode::CmdInsertNode(SoNode* node, QModelIndex& index, QUndoCommand* parent):
    QUndoCommand(parent),
    m_node(node)
{
    if (!m_node)
        gcf::SevereError("CmdInsertShapeKit called with NULL TShapeKit*");
    m_node->ref();

    m_model = (SceneModel*) index.model();
    if (!index.isValid())
        gcf::SevereError("CmdInsertShapeKit called with invalid ModelIndex.");
    InstanceNode* instance = m_model->getInstance(index);
    m_parent = static_cast<SoBaseKit*>(instance->getNode());

    setText(QString("Insert ") + node->getTypeId().getName().getString());
}

CmdInsertNode::~CmdInsertNode()
{
    m_node->unref();
}

void CmdInsertNode::undo()
{
    m_model->removeCoinNode(m_row, m_parent);
}

void CmdInsertNode::redo()
{
    m_row = m_model->insertCoinNode(m_node, m_parent);
}
