#pragma once

#include <QUndoCommand>

class AirKit;
class SceneTreeModel;


class CmdAirModified: public QUndoCommand
{
public:
    CmdAirModified(AirKit* air, SceneTreeModel* model, QUndoCommand* parent = 0);
    ~CmdAirModified();

     void undo();
     void redo();

private:
     AirKit* m_airOld;
     AirKit* m_air;
     SceneTreeModel* m_model;
};
