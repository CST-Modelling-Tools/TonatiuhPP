#pragma once

#include <QUndoCommand>

class SceneModel;
class SoSceneKit;
class TLightKit;
class ShapeAbstract;
class SunAbstract;

//!  CmdLightKitModified class is the light definition command stored in the command stack.
/*!
  CmdLightKitModified represents a modification of the scene light. The modification could be a new light definition,
  a modificaiton of the light position on the scene or change on the sun shape parameters value.
   \sa CmdLightPositionModified
*/

class CmdLightKitModified : public QUndoCommand
{
public:
    CmdLightKitModified( TLightKit* newLightKit, SoSceneKit* scene, SceneModel& sceneModel, QUndoCommand* parent = 0 );
    ~CmdLightKitModified();

     virtual void undo();
     virtual void redo();

private:
     bool m_previousLightKit;
     double m_previousAzimuth;
     QString m_previousDisbleNodes;
     double m_previousZenith;
     SunAbstract* m_pPreviousSunShape;
     TLightKit* m_pNewLightKit;
     SoSceneKit* m_scene;
     SceneModel* m_pModel;
};
