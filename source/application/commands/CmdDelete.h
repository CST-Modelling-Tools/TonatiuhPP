#pragma once

#include <QUndoCommand>

class QModelIndex;
class SceneModel;
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
    CmdDelete(const QModelIndex& index, SceneModel* model, QUndoCommand* parent = 0);
    ~CmdDelete();

    void undo();
    void redo();

private:
    SoNode* m_node;
    SoBaseKit* m_nodeParent;
    SceneModel* m_model;
    int m_row;
};
