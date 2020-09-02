#include "CmdSetFieldNode.h"

#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFNode.h>

#include "kernel/scene/TNode.h"
#include "libraries/math/gcf.h"


CmdSetFieldNode::CmdSetFieldNode(SoNode* node, QString field, TNode* value, QUndoCommand* parent):
    QUndoCommand(parent),
    m_node(node),
    m_field(field),
    m_value(value)
{
    SoSFNode* f = (SoSFNode*) m_node->getField(m_field.toLatin1().data());
    m_valueOld = (TNode*) f->getValue();
    m_valueOld->ref();

    m_value->ref();

    QString name = node->getTypeId().getName().getString();
    QString text = QString("%1.%2 -> %3").arg(name, m_field, m_value->getTypeName());
    setText(text);
}

CmdSetFieldNode::~CmdSetFieldNode()
{
    m_value->unref();
    m_valueOld->unref();
}

void CmdSetFieldNode::undo()
{
    SoSFNode* f = (SoSFNode*) m_node->getField(m_field.toLatin1().data());
    f->setValue(m_valueOld);
}

void CmdSetFieldNode::redo()
{
    SoSFNode* f = (SoSFNode*) m_node->getField(m_field.toLatin1().data());
    f->setValue(m_value);
}
