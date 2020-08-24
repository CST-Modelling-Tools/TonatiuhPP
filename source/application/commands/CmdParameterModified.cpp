#include "CmdParameterModified.h"

#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/SbString.h>

#include "libraries/math/gcf.h"
//#include "application/tree/SceneModel.h"


CmdParameterModified::CmdParameterModified(
    SoNode* node, QString parameter, QString value,
    SceneModel* model, QUndoCommand* parent
):
    QUndoCommand(parent),
    m_node(node),
    m_parameter(parameter),
    m_value(value),
    m_model(model)
{
    if (!m_node)
        gcf::SevereError("CmdModifyParameter called with invalid node.");

    SoField* field = m_node->getField(m_parameter.toStdString().c_str());
    if (!field)
        gcf::SevereError("CmdModifyParameter called with invalid parameterName.");

    SbString fieldValue;
    field->get(fieldValue);
    m_valueOld = fieldValue.getString();

    QString nodeName = node->getTypeId().getName().getString();
    QString text = QString("Parameter modified: %1.%2 = %3").arg(nodeName, parameter, value);
    setText(text);
}

void CmdParameterModified::undo()
{
    SoField* field = m_node->getField(m_parameter.toStdString().c_str());
    field->set(m_valueOld.toStdString().c_str());
}

void CmdParameterModified::redo()
{
    SoField* field = m_node->getField(m_parameter.toStdString().c_str());
    field->set(m_value.toStdString().c_str());
}
