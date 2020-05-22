#pragma once

#include <QUndoCommand>
#include <QModelIndex>

class InstanceNode;
class Tracker;
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
        Tracker* tracker,
        const QModelIndex& parentIndex,
        SceneModel* model,
        QUndoCommand* parent = 0
    );
    ~CmdInsertTracker();

    void undo();
    void redo();

private:
    Tracker* m_tracker;
    SoBaseKit* m_coinParent;
    SceneModel* m_model;
    int m_row;
};
