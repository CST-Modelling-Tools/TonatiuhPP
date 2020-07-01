#include "CmdChangeNodeName.h"

/*!
 * \class CmdChangeNodeName
 * \brief CmdChangeNodeName is the command that represents a node name change.
 *
 *CmdChangeNodeName represents a change in a node from the scene. The node can be any one.
 */

#include <QModelIndex>
#include <QString>

#include <Inventor/nodes/SoNode.h>

#include "libraries/math/gcf.h"
#include "kernel/run/InstanceNode.h"
#include "tree/SceneModel.h"

/*!
 * Creates a new CmdChangeNodeName command.
 */
CmdChangeNodeName::CmdChangeNodeName(
    const QModelIndex& index,
    QString name,
    SceneModel* model,
    QUndoCommand* parent
):
    QUndoCommand(parent),
    m_name(name),
    m_model(model)
{
    if( !index.isValid() )
        gcf::SevereError("CmdChangeNodeName called with invalid ModelIndex.");

    InstanceNode* instance = m_model->getInstance(index);
    if (!instance)
        gcf::SevereError("CmdChangeNodeName called with invalid node." );

    m_node = instance->getNode();
    m_nameOld = m_node->getName().getString();

    QString text = QString("Node name changed to: %1").arg(name);
    setText(text);
}

/*!
 * Changes the node name to the name that the node takes before the command is applied.
 */
void CmdChangeNodeName::undo()
{
    m_model->SetNodeName(m_node, m_nameOld);
}

/*!
 * Changes the node name to the name defined in the command constructor.
 */
void CmdChangeNodeName::redo()
{
    m_model->SetNodeName(m_node, m_name);
}
