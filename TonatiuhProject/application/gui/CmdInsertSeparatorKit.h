#pragma once

#include <QUndoCommand>

class InstanceNode;
class QModelIndex;
class TSeparatorKit;
class SceneModel;

//! CmdInsertSeparatorKit class is the insert command for TSeparatorKit nodes stored in the command stack.
/*!
   CmdInsertSeparatorKit represents a single TSeparatorKit insertion action on a scene, insert a new TSeparatorKit node to the scene.
 */

class CmdInsertSeparatorKit: public QUndoCommand
{
public:
    CmdInsertSeparatorKit(TSeparatorKit* separatorKit,  const QModelIndex& parentIndex, SceneModel* model, QUndoCommand* parent = 0);
    ~CmdInsertSeparatorKit();

    virtual void undo();
    virtual void redo();

private:
    TSeparatorKit* m_separatorKit;
    SoBaseKit* m_coinParent;
    SceneModel* m_pModel;
    int m_row;

};
