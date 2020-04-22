#pragma once

#include <QUndoCommand>

class QModelIndex;
class SoSceneKit;
class SceneModel;
class TSeparatorKit;
class TTracker;

//!  CmdDeleteTracker class is the delete command for trackers nodes stored in the command stack.
/*!
   CmdDeleteTracker represents a single tracker node delete action on a scene. Deletes a node from the scene.
   \sa CmdDelete, CmdCopy, CmdCut, CmdPaste
 */

class CmdDeleteTracker: public QUndoCommand
{
public:
    CmdDeleteTracker(const QModelIndex& selectedIndex, SoSceneKit* scene, SceneModel& model, QUndoCommand* parent = 0);
    ~CmdDeleteTracker();

    virtual void undo();
    virtual void redo();

private:
    TTracker* m_tracker;
    TSeparatorKit* m_coinParent;
    SoSceneKit* m_scene;
    SceneModel* m_pModel;
    int m_row;
};
