#pragma once

//!  CmdTransmissivityModified class is the transmissivity definition command stored in the command stack.
/*!
  CmdTransmissivityModified represents a modification of the scene transmissivity. The modification could be a new transmissivity definition,
  or a modificaiton of the transmissivity parameters values.
*/

#include <QUndoCommand>

class TSceneKit;
class Air;

class CmdAirModified: public QUndoCommand
{
public:
    CmdAirModified(Air* airNew, TSceneKit* scene, QUndoCommand* parent = 0);
    ~CmdAirModified();

     void undo();
     void redo();

private:
     bool m_hasOld;
     Air* m_airOld;
     Air* m_airNew;
     TSceneKit* m_scene;
};
