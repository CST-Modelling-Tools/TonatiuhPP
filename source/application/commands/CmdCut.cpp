#include <Inventor/nodekits/SoBaseKit.h>

#include "kernel/run/InstanceNode.h"
#include "CmdCut.h"
#include "tree/SceneModel.h"

/**
 * Creates a new cut command that represents the cut the node located with \a index from the \a model.
 * The node is stored at the \a clipborad.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdCut::CmdCut(const QModelIndex& selectedIndex, SoNode*& clipboard, SceneModel* model, QUndoCommand* parent)
    : QUndoCommand("Cut", parent), m_pClipboard (clipboard), m_previousNode (0), m_coinNode(0), m_coinParent(0), m_pModel(model), m_row (-1)
{
    InstanceNode* instanceNode = m_pModel->NodeFromIndex(selectedIndex);
    m_coinNode = instanceNode->GetNode();
    m_coinNode->ref();
    m_coinParent = static_cast< SoBaseKit* > (instanceNode->GetParent()->GetNode() );

    m_previousNode = clipboard;

    m_row = instanceNode->GetParent()->children.indexOf(instanceNode);
}

/*!
 * Destroys the CmdCut object.
 */
CmdCut::~CmdCut()
{
    m_coinNode->unref();
}

/*!
 * Reverts clipboard and scene contain. After undo() is called, the state of the scene and clipboard will be the same as before redo() was called.
 * \sa redo().
 */
void CmdCut::undo()
{
    m_pClipboard = m_previousNode;
    m_pModel->Paste(tgc::Shared, *m_coinParent, *m_coinNode, m_row);
}

/*!
 * Applies a change to the scene and clipboard. After redo() clipboard will contain the node located on the model as index
 * and the node will remove from the scene.
 * \sa undo().
 */
void CmdCut::redo()
{
    m_pClipboard = m_coinNode;
    m_pModel->Cut(*m_coinParent, m_row);
}
