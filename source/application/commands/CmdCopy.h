#pragma once
#include <QUndoCommand>

class QModelIndex;
class SoBaseKit;
class SceneModel;
class SoNode;

//!  CmdCopy class is the copy command stored in the command stack.
/*!
   CmdCopy represents a single copy action on a scene, copy on a clipboard a node from the scene.
   \sa CmdCut, CmdPaste, CmdDelete
 */

class CmdCopy: public QUndoCommand
{
public:
    CmdCopy(const QModelIndex& index, SoNode*& clipboard, SceneModel* model, QUndoCommand* parent = 0);
    ~CmdCopy() {}

    virtual void undo();
    virtual void redo();

private:
    SoNode*& m_pClipboard; /*!< The Clipboard */
    SoNode* m_pNode; /*!< Copied node to the clipboard*/
    SoNode* m_previousNode; /*!< The node saved on the clipboard befor apply copy*/
    SceneModel* m_pModel; /*!< The scene */
};
