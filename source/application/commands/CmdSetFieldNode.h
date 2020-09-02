#pragma once

#include <QUndoCommand>

class SoNode;
class TNode;


class CmdSetFieldNode: public QUndoCommand
{
public:
    CmdSetFieldNode(SoNode* node, QString field, TNode* value, QUndoCommand* parent = 0);
    ~CmdSetFieldNode();

    void undo();
    void redo();

private:
    SoNode* m_node;
    QString m_field;
    TNode* m_value;
    TNode* m_valueOld;
};
