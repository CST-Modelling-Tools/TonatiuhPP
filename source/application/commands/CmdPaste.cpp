#include <Inventor/nodekits/SoBaseKit.h>

#include "libraries/math/gcf.h"

#include "CmdPaste.h"
#include "tree/SceneModel.h"
#include "kernel/run/InstanceNode.h"

/**
 * Creates a new paste command that represents a \a type paste of the \a coinClipboard node as a node located with \a parentModelIndex child in the \a model.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdPaste::CmdPaste(tgc::PasteType type,
                   const QModelIndex& indexParent,
                   SoNode*& node,
                   SceneModel* model,
                   QUndoCommand* parent):
    QUndoCommand("Paste", parent),
    m_pasteType(type),
    m_parentInstance(0),
    m_row(-1),
    m_node(node),
    m_model(model)
{
    m_nameOld = node->getName().getString();

    if (!indexParent.isValid())
        gcf::SevereError("CmdPaste called with invalid ModelIndex.");

    m_parentInstance = m_model->getInstance(indexParent);
    if (!m_parentInstance->getNode())
        gcf::SevereError("CmdPaste NULL m_coinParent.");

    m_row = m_parentInstance->children.size();
}

CmdPaste::~CmdPaste()
{

}

void CmdPaste::undo()
{
    SoBaseKit* nodeParent = static_cast<SoBaseKit*> ( m_parentInstance->getNode() );
    m_model->Cut(*nodeParent, m_row);
    m_parentInstance->children[m_row]->getNode()->unref();
    m_model->setNodeName(m_node, m_nameOld);
}

void CmdPaste::redo()
{
    SoBaseKit* coinParent = static_cast<SoBaseKit*> ( m_parentInstance->getNode() );
    if ( !m_model->Paste(m_pasteType, *coinParent, *m_node, m_row) ) return;

    SoNode* node = m_parentInstance->children[m_row]->getNode();
    node->ref();

    QString name = m_nameOld;
    int count = 0;
    while (!m_model->setNodeName(node, name))
        name = m_nameOld + QString("_copy_%1").arg(++count);
}
