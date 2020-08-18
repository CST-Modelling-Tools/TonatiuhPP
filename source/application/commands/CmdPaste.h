#pragma once

#include <QUndoCommand>

#include "commands/tgc.h"

class InstanceNode;
class QModelIndex;
class SceneModel;
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
    CmdPaste(tgc::PasteType type, const QModelIndex& indexParent, SoNode*& node, SceneModel* model, QUndoCommand* parent = 0 );
    ~CmdPaste();

     void undo();
     void redo();

private:
    tgc::PasteType m_pasteType;
    InstanceNode* m_parentInstance;
    int m_row;
    QString m_nameOld;
    SoNode* m_node;
    SceneModel* m_model;
};
