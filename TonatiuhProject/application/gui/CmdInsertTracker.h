#pragma once

#include <QUndoCommand>
#include <QModelIndex>

class InstanceNode;
class TTracker;
class SceneModel;
class SoSceneKit;

//!  CmdInsertTracker class is the insert command for trackers stored in the command stack.
/*!
  CmdInsertTracker represents a single shape insertion action on a scene, insert a new trackers node to the scene.
*/

class CmdInsertTracker : public QUndoCommand
{
public:
    CmdInsertTracker( TTracker* tracker, const QModelIndex& parentIndex, SoSceneKit* scene, SceneModel* model, QUndoCommand* parent = 0 );
    ~CmdInsertTracker();

    virtual void undo();
    virtual void redo();

private:

    TTracker* m_tracker;
    SoBaseKit* m_coinParent;;
    SoSceneKit* m_scene;
    SceneModel* m_pModel;
    int m_row;
     
};
