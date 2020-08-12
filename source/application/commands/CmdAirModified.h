#pragma once

#include <QUndoCommand>

class AirKit;
class SceneModel;

class CmdAirModified: public QUndoCommand
{
public:
    CmdAirModified(AirKit* air, SceneModel* model, QUndoCommand* parent = 0);
    ~CmdAirModified();

     void undo();
     void redo();

private:
     AirKit* m_airOld;
     AirKit* m_air;
     SceneModel* m_model;
};
