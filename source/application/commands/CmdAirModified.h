#pragma once

//!  CmdTransmissivityModified class is the transmissivity definition command stored in the command stack.
/*!
  CmdTransmissivityModified represents a modification of the scene transmissivity. The modification could be a new transmissivity definition,
  or a modificaiton of the transmissivity parameters values.
*/

#include <QUndoCommand>

class TSceneKit;
class AirAbstract;

class CmdAirModified: public QUndoCommand
{
public:
    CmdAirModified(AirAbstract* airNew, TSceneKit* scene, QUndoCommand* parent = 0);
    ~CmdAirModified();

     void undo();
     void redo();

private:
     bool m_hasOld;
     AirAbstract* m_airOld;
     AirAbstract* m_airNew;
     TSceneKit* m_scene;
};
