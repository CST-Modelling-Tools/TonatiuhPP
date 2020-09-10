#pragma once

#include <QUndoCommand>

class InstanceNode;
class QModelIndex;
class SceneTreeModel;
class SoBaseKit;
class SoNode;

//!  CmdPaste class is the paste command stored in the command stack.
/*!
  CmdPaste represents a single paste action on a scene, paste the node on the clipboard to selected node in the scene.
    If the tgc::Shared parameter defined to the command, the node at the clipboard is shared between the previous parents and the new parent. tgc::Copy to create a new node before the paste.
   \sa CmdCopy, CmdCut, CmdDelete
*/

class CmdPaste: public QUndoCommand
{
public:
    CmdPaste(bool isShared, const QModelIndex& index, SoNode*& node, SceneTreeModel* model, QUndoCommand* parent = 0);
    ~CmdPaste();

     void undo();
     void redo();

private:
    bool m_isShared;
    InstanceNode* m_instance;
    int m_row;
    QString m_name;
    SoNode* m_node;
    SceneTreeModel* m_model;
};
