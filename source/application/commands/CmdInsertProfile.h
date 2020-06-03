#pragma once

#include <QUndoCommand>

class SceneModel;
class TShapeKit;
class ProfileRT;

//!  CmdInsertShape class is the insert command for shapes stored in the command stack.
/*!
   CmdInsertShape represents a single shape insertion action on a scene, insert a new shape node to the scene.
 */

class CmdInsertProfile: public QUndoCommand
{
public:
    CmdInsertProfile(TShapeKit* shapeKit, ProfileRT* shape, SceneModel* model, QUndoCommand* parent = 0);
    ~CmdInsertProfile();

    void undo();
    void redo();

private:
    TShapeKit* m_shapeKit;
    ProfileRT* m_profileOld;
    ProfileRT* m_profile;
    SceneModel* m_model;
};
