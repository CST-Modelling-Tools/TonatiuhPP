#pragma once

#include <QUndoCommand>
#include <QModelIndex>

class InstanceNode;
class TrackerAbstract;
class SceneModel;
class SoSceneKit;
class SoBaseKit;

//!  CmdInsertTracker class is the insert command for trackers stored in the command stack.
/*!
  CmdInsertTracker represents a single shape insertion action on a scene, insert a new trackers node to the scene.
*/

class CmdInsertTracker: public QUndoCommand
{
public:
    CmdInsertTracker(
        TrackerAbstract* tracker,
        const QModelIndex& parentIndex,
        SoSceneKit* scene,
        SceneModel* model,
        QUndoCommand* parent = 0
    );
    ~CmdInsertTracker();

    void undo();
    void redo();

private:
    TrackerAbstract* m_tracker;
    SoBaseKit* m_coinParent;
    SoSceneKit* m_scene;
    SceneModel* m_model;
    int m_row;
};
