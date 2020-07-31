#pragma once

#include <QUndoCommand>

class TSceneKit;
class AirTransmission;

//!  CmdTransmissivityModified class is the transmissivity definition command stored in the command stack.
/*!
  CmdTransmissivityModified represents a modification of the scene transmissivity. The modification could be a new transmissivity definition,
  or a modificaiton of the transmissivity parameters values.
*/
class CmdAirModified: public QUndoCommand
{
public:
    CmdAirModified(AirTransmission* air, TSceneKit* sceneKit, QUndoCommand* parent = 0);
    ~CmdAirModified();

     void undo();
     void redo();

private:
     AirTransmission* m_airOld;
     AirTransmission* m_air;
     TSceneKit* m_sceneKit;
};
