#pragma once

#include <QUndoCommand>
#include <QModelIndex>

class InstanceNode;
class TrackerKit;
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
    CmdInsertTracker(TrackerKit* tracker, const QModelIndex& index,
        SceneModel* model, QUndoCommand* parent = 0
    );
    ~CmdInsertTracker();

    void undo();
    void redo();

private:
    TrackerKit* m_tracker;
    SoBaseKit* m_kit;
    SceneModel* m_model;
    int m_row;
};
