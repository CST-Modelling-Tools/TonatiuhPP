#pragma once

//!  CmdTransmissivityModified class is the transmissivity definition command stored in the command stack.
/*!
  CmdTransmissivityModified represents a modification of the scene transmissivity. The modification could be a new transmissivity definition,
  or a modificaiton of the transmissivity parameters values.
*/

#include <QUndoCommand>

class TSceneKit;
class AirAbstract;

class CmdTransmissivityModified : public QUndoCommand
{

public:
    CmdTransmissivityModified( AirAbstract* newTransmissivity, TSceneKit* scene,  QUndoCommand* parent = 0 );
    ~CmdTransmissivityModified();

     virtual void undo();
     virtual void redo();

private:
     bool m_isPreviousTransmissivity;
     AirAbstract* m_pPreviousTransmissivity;
     AirAbstract* m_pNewTransmissivity;
     TSceneKit* m_scene;
};

