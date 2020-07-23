#pragma once

#include <QUndoCommand>

class TSeparatorKit;
class QModelIndex;
class SoBaseKit;
class SceneModel;

//! CmdInsertSeparatorKit class is the insert command for TSeparatorKit nodes stored in the command stack.
/*!
   CmdInsertSeparatorKit represents a single TSeparatorKit insertion action on a scene, insert a new TSeparatorKit node to the scene.
 */
class CmdCreateNode: public QUndoCommand
{
public:
    CmdCreateNode(
        TSeparatorKit* separatorKit,
        const QModelIndex& parentIndex,
        SceneModel* model,
        QUndoCommand* parent = 0
    );
    ~CmdCreateNode();

    void undo();
    void redo();

private:
    TSeparatorKit* m_node;
    SoBaseKit* m_nodeParent;
    SceneModel* m_model;
    int m_row;
};
