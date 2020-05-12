#include "libraries/geometry/gf.h"

#include <Inventor/nodekits/SoBaseKit.h>
#include "CmdInsertSeparatorKit.h"
#include "kernel/run/InstanceNode.h"
#include "tree/SceneModel.h"
#include "kernel/scene/TSeparatorKit.h"

/**
 * Creates a new group node insert command that adds a \a separatorKit to \a parentIndex node in the \a model.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdInsertSeparatorKit::CmdInsertSeparatorKit( TSeparatorKit* separatorKit,  const QModelIndex& parentIndex, SceneModel* model, QUndoCommand* parent ):
    QUndoCommand("InsertSeparatorKit", parent), m_separatorKit ( separatorKit ), m_coinParent( 0 ), m_pModel( model ), m_row( -1 )
{
    if( !m_separatorKit ) gf::SevereError( "CmdInsertSeparatorKit Null separatorKit." );
    m_separatorKit->ref();

    if( !parentIndex.isValid() ) gf::SevereError( "CmdInsertSeparatorKit called with invalid ModelIndex." );
    InstanceNode* instanceParent = m_pModel->NodeFromIndex( parentIndex );
    m_coinParent = static_cast< SoBaseKit* > ( instanceParent->getNode() );
}

/*!
 * Destroys the CmdInsertSeparatorKit object.
 */
CmdInsertSeparatorKit::~CmdInsertSeparatorKit()
{
    m_separatorKit->unref();
}

/*!
 * Reverts model state. After undo() is called, the node with the parentIndex will not contain a separatorKit node.
 * \sa redo().
 */
void CmdInsertSeparatorKit::undo()
{
    m_pModel->RemoveCoinNode(m_row, *m_coinParent);
}

/*!
 * Applies a change to the model. After redo() the model will contain new group node.
 * \sa undo().
 */
void CmdInsertSeparatorKit::redo()
{
    m_row = m_pModel->InsertCoinNode(*m_separatorKit, *m_coinParent);
}
