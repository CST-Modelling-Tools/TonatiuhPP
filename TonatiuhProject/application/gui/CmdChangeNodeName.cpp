/*!
 * \class CmdChangeNodeName
 * \brief CmdChangeNodeName is the command that represents a node name change.
 *
 *CmdChangeNodeName represents a change in a node from the scene. The node can be any one.
 */

#include <QModelIndex>
#include <QString>

#include <Inventor/SbName.h>
#include <Inventor/nodes/SoNode.h>

#include "gf.h"

#include "CmdChangeNodeName.h"
#include "kernel/gui/InstanceNode.h"
#include "SceneModel.h"

/*!
 * Creates a new CmdChangeNodeName command.
 */
CmdChangeNodeName::CmdChangeNodeName( const QModelIndex& index, QString newName, SceneModel* model, QUndoCommand* parent )
: QUndoCommand("Change node name", parent), m_newName ( newName ), m_previousName( "" ), m_pNode( 0 ), m_pModel( model )
{
    if( !index.isValid() ) gf::SevereError( "CmdChangeNodeName called with invalid ModelIndex." );
    InstanceNode* selectedNodeInstance = m_pModel->NodeFromIndex( index );
    if( !selectedNodeInstance ) gf::SevereError( "CmdChangeNodeName called with invalid node." );
    m_pNode = selectedNodeInstance->GetNode();

    m_previousName = QString( m_pNode->getName().getString() );

}

/*!
 * Changes the node name to the name that the node takes before the command is applied.
 */
void CmdChangeNodeName::undo()
{
    m_pModel->SetNodeName( m_pNode, m_previousName );
}

/*!
 * Changes the node name to the name defined in the command constructor.
 */
void CmdChangeNodeName::redo()
{
    m_pModel->SetNodeName( m_pNode, m_newName );
}
