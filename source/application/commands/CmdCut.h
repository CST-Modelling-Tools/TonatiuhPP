#pragma once

#include <QUndoCommand>

class QModelIndex;
class SceneModel;
class SoNode;
class SoBaseKit;

//!  CmdCut class is the cut command stored in the command stack.
/*!
   CmdCut represents a single cut action on a scene, removes selected node from the scene and stores it in the clipboard.
   \sa CmdCopy, CmdPaste, CmdDelete
 */

class CmdCut: public QUndoCommand
{
public:
    CmdCut(const QModelIndex& index, SoNode*& clipboard, SceneModel* model, QUndoCommand* parent = 0);
    ~CmdCut();

    void undo();
    void redo();

private:
    SoNode*& m_clipboard;
    SceneModel* m_model;

    SoNode* m_nodeOld;
    SoNode* m_node;
    SoBaseKit* m_parent;
    int m_row;
};
