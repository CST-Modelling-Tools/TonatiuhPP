#pragma once

#include <QUndoCommand>

class SceneModel;
class TSceneKit;
class SunKit;

//!  CmdLightKitModified class is the light definition command stored in the command stack.
/*!
  CmdLightKitModified represents a modification of the scene light. The modification could be a new light definition,
  a modificaiton of the light position on the scene or change on the sun shape parameters value.
   \sa CmdLightPositionModified
*/
class CmdSunModified: public QUndoCommand
{
public:
    CmdSunModified(
        SunKit* sunKit,
        TSceneKit* sceneKit,
        SceneModel* model,
        QUndoCommand* parent = 0
     );
    ~CmdSunModified();

     void undo();
     void redo();

private:
     SunKit* m_sunKitOld;
     SunKit* m_sunKit;
     TSceneKit* m_sceneKit;
     SceneModel* m_model;
};
