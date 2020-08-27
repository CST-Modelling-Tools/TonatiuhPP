#pragma once

#include <QUndoCommand>

class SoNode;
class QModelIndex;
class SceneModel;
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
    SceneModel* m_model;
    int m_row;
};
