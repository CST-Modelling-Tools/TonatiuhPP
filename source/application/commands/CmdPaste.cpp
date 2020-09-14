#include <Inventor/nodekits/SoBaseKit.h>

#include "libraries/math/gcf.h"

#include "CmdPaste.h"
#include "tree/SceneTreeModel.h"
#include "kernel/run/InstanceNode.h"

/**
 * Creates a new paste command that represents a \a type paste of the \a coinClipboard node as a node located with \a parentModelIndex child in the \a model.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdPaste::CmdPaste(bool isShared,
                   const QModelIndex& index,
                   SoNode*& node,
                   SceneTreeModel* model,
                   QUndoCommand* parent):
    QUndoCommand("Paste", parent),
    m_isShared(isShared),
    m_row(-1),
    m_node(node),
    m_model(model)
{
    m_name = node->getName().getString();

    if (!index.isValid())
        gcf::SevereError("CmdPaste called with invalid ModelIndex.");

    m_instance = m_model->getInstance(index);
    if (!m_instance->getNode())
        gcf::SevereError("CmdPaste NULL m_coinParent.");

    m_row = m_instance->children.size();
}

CmdPaste::~CmdPaste()
{

}

void CmdPaste::undo()
{
    SoBaseKit* nodeParent = static_cast<SoBaseKit*> (m_instance->getNode());
    m_model->Cut(nodeParent, m_row);
    m_instance->children[m_row]->getNode()->unref();
    m_model->setNodeName(m_node, m_name);
}

void CmdPaste::redo()
{
    SoBaseKit* kit = static_cast<SoBaseKit*> ( m_instance->getNode() );
    m_model->Paste(kit, m_node, m_row, m_isShared);

    SoNode* node = m_instance->children[m_row]->getNode();
    node->ref();

    QString name = m_name;
    int count = 0;
    while (!m_model->setNodeName(node, name))
        name = m_name + QString("_copy_%1").arg(++count);
}
