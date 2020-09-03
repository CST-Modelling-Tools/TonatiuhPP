#pragma once

#include <QUndoCommand>

class QModelIndex;
class SoNode;
class SceneTreeModel;


class CmdCopy: public QUndoCommand
{
public:
    CmdCopy(const QModelIndex& index, SoNode*& clipboard, SceneTreeModel* model, QUndoCommand* parent = 0);
    ~CmdCopy() {}

    void undo();
    void redo();

private:
    SoNode*& m_clipboard;
    SoNode* m_nodeOld; // Node saved in clipboard before
    SoNode* m_node; // Node copied to clipboard
    SceneTreeModel* m_model;
};
