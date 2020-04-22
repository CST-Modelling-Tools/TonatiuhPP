#pragma once
#include <QUndoCommand>

class QModelIndex;
class SceneModel;


//!  CmdDelete class is the delete command stored in the command stack.
/*!
   CmdDelete represents a single node delete action on a scene. Deletes a node from the scene and all children.
   \sa CmdCopy, CmdCut, CmdPaste
 */

class CmdDelete: public QUndoCommand
{
public:
    CmdDelete(const QModelIndex& selectedIndex, SceneModel& model, QUndoCommand* parent = 0);
    ~CmdDelete();

    virtual void undo();
    virtual void redo();

private:
    SoNode* m_coinNode;
    SoBaseKit* m_coinParent;
    SceneModel* m_pModel;
    int m_row;
};
