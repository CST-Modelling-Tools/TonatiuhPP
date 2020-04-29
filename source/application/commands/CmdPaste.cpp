#include <Inventor/nodekits/SoBaseKit.h>

#include "libraries/geometry/gf.h"

#include "CmdPaste.h"
#include "gui/SceneModel.h"
#include "kernel/run/InstanceNode.h"

/**
 * Creates a new paste command that represents a \a type paste of the \a coinClipboard node as a node located with \a parentModelIndex child in the \a model.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdPaste::CmdPaste( tgc::PasteType type, const QModelIndex& parentModelIndex,  SoNode*& coinClipboard, SceneModel& sceneModel, QUndoCommand* parent )
: QUndoCommand("Paste", parent), m_pasteType( type ), m_parentInstance( 0 ), m_coinChild( coinClipboard ), m_sceneModel( &sceneModel ), m_oldNodeName( "" ),  m_row( -1 )
{
    if( !parentModelIndex.isValid() ) gf::SevereError( "CmdPaste called with invalid ModelIndex." );

    m_parentInstance = m_sceneModel->NodeFromIndex( parentModelIndex );
    if( !m_parentInstance-> GetNode() ) gf::SevereError( "CmdPaste NULL m_coinParent." );

    m_row = m_parentInstance->children.size();
    m_oldNodeName = QString( coinClipboard->getName().getString() );
}

/*!
 * Destroys the CmdPaste object.
 */
CmdPaste::~CmdPaste()
{

}

/*!
 * Reverts scene state. After undo() is called, the node will remove from the scene.
 * \sa redo().
 */
void CmdPaste::undo()
{
    SoBaseKit* coinParent = static_cast< SoBaseKit* > ( m_parentInstance-> GetNode() );
    m_sceneModel->Cut( *coinParent, m_row );
    m_parentInstance->children[m_row]->GetNode()->unref();
    m_sceneModel->SetNodeName( m_coinChild, m_oldNodeName );
}

/*!
 * Applies a change to the scene. After redo() parentIndex contains a new child.
 * \sa undo().
 */
void CmdPaste::redo( )
{
    SoBaseKit* coinParent = static_cast< SoBaseKit* > ( m_parentInstance-> GetNode() );
    if( !m_sceneModel->Paste( m_pasteType, *coinParent, *m_coinChild, m_row ) ) return;

    SoNode* newNode = m_parentInstance->children[m_row]->GetNode();
    newNode->ref();

    int count = 0;
    QString newName = m_oldNodeName;
    while ( !m_sceneModel->SetNodeName( newNode, newName ) )
    {
        count++;
        newName = QString( "%1_copy%2").arg( m_oldNodeName, QString::number( count) );
    }
}
