#pragma once

#include <QUndoCommand>

class QModelIndex;
class SceneTreeModel;
class SoNode;
class SoBaseKit;


//!  CmdDelete class is the delete command stored in the command stack.
/*!
   CmdDelete represents a single node delete action on a scene. Deletes a node from the scene and all children.
   \sa CmdCopy, CmdCut, CmdPaste
 */
class CmdDelete: public QUndoCommand
{
public:
    CmdDelete(const QModelIndex& index, SceneTreeModel* model, QUndoCommand* parent = 0);
    ~CmdDelete();

    void undo();
    void redo();

private:
    SceneTreeModel* m_model;
    SoNode* m_node;
    SoBaseKit* m_parent;
    int m_row;
};
