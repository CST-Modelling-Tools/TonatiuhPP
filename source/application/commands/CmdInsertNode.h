#pragma once

#include <QUndoCommand>

class SoNode;
class QModelIndex;
class SceneTreeModel;
class SoBaseKit;


class CmdInsertNode: public QUndoCommand
{
public:
    CmdInsertNode(SoNode* node, QModelIndex& index, QUndoCommand* parent = 0);
    ~CmdInsertNode();

    void undo();
    void redo();

private:
    SoNode* m_node;
    SoBaseKit* m_parent;
    SceneTreeModel* m_model;
    int m_row;
};
