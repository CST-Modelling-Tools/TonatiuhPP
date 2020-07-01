//!  CmdModifyParameter class is the parameter modification command stored in the command stack.
/*!
  CmdModifyParameter represents a modification of the scene element parameter. The modification could be for any element of the scene concentrator parameters.
  This class saves the new value of the parameter and old one to restore the previous state.
   \sa CmdLightPositionModified
*/

#include <Inventor/SbName.h>
#include <Inventor/SbString.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/nodes/SoNode.h>

#include "libraries/math/gcf.h"

#include "CmdModifyParameter.h"
#include "kernel/run/InstanceNode.h"
#include "tree/SceneModel.h"

/**
 * CmdModifyParameter creates a new command that represents a scene node parameter value modification. This object saves the \a parameterName parameter value of \a nodeIndex node and the new one
 * defined as \a parameterValue to apply and restore them.
 *
 * The \a parent is command object parent.
 */
CmdModifyParameter::CmdModifyParameter(SoNode* node, QString name, QString value, SceneModel* model, QUndoCommand* parent ):
    QUndoCommand(parent),
    m_coinNode(node),
    m_newValue (value),
    m_oldValue(""),
    m_parameterName(name),
    m_pModel(model)
{
    if (!m_coinNode)
        gcf::SevereError("CmdModifyParameter called with invalid node." );

    SoField* parameterField = m_coinNode->getField( SbName( m_parameterName.toStdString().c_str() ) );
    if (!parameterField)
        gcf::SevereError( "CmdModifyParameter called with invalid parameterName." );

    SbString fieldValue;
    parameterField->get(fieldValue);
    m_oldValue = QString(fieldValue.getString());
}

/*!
 * Applies a change to the scene. After undo() the node parameter value will be changed to \a m_oldValue defined in the constructor.
 * \sa undo().
 */
void CmdModifyParameter::undo()
{
    SoField* field = m_coinNode->getField( m_parameterName.toStdString().c_str() );
    if (field)
        field->set( m_oldValue.toStdString().c_str() );
}

/*!
 * Applies a change to the scene. After redo() the node parameter value will be changed to \a m_newValue defined in the constructor.
 * \sa undo().
 */
void CmdModifyParameter::redo()
{
    SoField* field = m_coinNode->getField(m_parameterName.toStdString().c_str());
    if (field)
        field->set(m_newValue.toStdString().c_str());
}

