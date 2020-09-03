#pragma once

#include <QUndoCommand>

class SoNode;


class CmdSetFieldNode: public QUndoCommand
{
public:
    CmdSetFieldNode(SoNode* node, QString field, SoNode* value, QUndoCommand* parent = 0);
    ~CmdSetFieldNode();

    void undo();
    void redo();

private:
    SoNode* m_node;
    QString m_field;
    SoNode* m_value;
    SoNode* m_valueOld;
};
