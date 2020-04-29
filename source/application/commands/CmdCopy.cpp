#include <Inventor/nodekits/SoBaseKit.h>

#include "CmdCopy.h"
#include "kernel/run/InstanceNode.h"
#include "gui/SceneModel.h"

/**
 * Creates a new copy command that represents the copy of the node located with \a index in the \a model to the \a clipborad.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdCopy::CmdCopy( const QModelIndex& index, SoNode*& clipboard, SceneModel* model, QUndoCommand* parent)
: QUndoCommand("Copy", parent), m_pClipboard ( clipboard ), m_pNode( 0 ),m_previousNode ( 0 ), m_pModel( model )
{
    InstanceNode* instanceNode = m_pModel->NodeFromIndex( index );
    m_pNode = instanceNode->GetNode();

    m_previousNode = clipboard ;
}

/*!
 * Reverts clipboard contain. After undo() is called, the state of the clipboard will be the same as before redo() was called.
 * \sa redo().
 */
void CmdCopy::undo()
{
    m_pClipboard = m_previousNode;
}

/*!
 * Applies a change to the clipboard. After redo() clipboard will contain the node located on the model as index
 * \sa undo().
 */
void CmdCopy::redo( )
{
    m_pClipboard  = m_pNode;
}
