#include "CmdInsertNode.h"

#include <Inventor/nodekits/SoBaseKit.h>

#include "libraries/math/gcf.h"
#include "kernel/run/InstanceNode.h"
#include "kernel/scene/TSeparatorKit.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/trackers/TrackerKit.h"
#include "tree/SceneTreeModel.h"


CmdInsertNode::CmdInsertNode(SoNode* node, QModelIndex& index, QUndoCommand* parent):
    QUndoCommand(parent),
    m_node(node)
{
    if (!m_node)
        gcf::SevereError("CmdInsertShapeKit called with NULL TShapeKit*");
    m_node->ref();

    m_model = (SceneTreeModel*) index.model();
    if (!index.isValid())
        gcf::SevereError("CmdInsertShapeKit called with invalid ModelIndex.");
    InstanceNode* instance = m_model->getInstance(index);
    m_parent = static_cast<SoBaseKit*>(instance->getNode());

    QString type;
    if (dynamic_cast<TSeparatorKit*>(node))
        type = "Node";
    else if (dynamic_cast<TShapeKit*>(node))
        type = "Shape";
    else if (dynamic_cast<TrackerKit*>(node))
        type = "Tracker";
    else
        type = node->getTypeId().getName().getString();

    QString text = QString("node.create%1();").arg(type);
    setText(text);
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
