#pragma once

#include <QUndoCommand>

class SunKit;
class SceneTreeModel;


class CmdSunModified: public QUndoCommand
{
public:
    CmdSunModified(SunKit* sun, SceneTreeModel* model, QUndoCommand* parent = 0);
    ~CmdSunModified();

     void undo();
     void redo();

private:
     void set(SunKit* sun);

private:
     SunKit* m_sunOld;
     SunKit* m_sun;
     SceneTreeModel* m_model;
};
