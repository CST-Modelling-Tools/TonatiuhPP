#pragma once

#include <QUndoCommand>

class SunKit;
class SceneModel;

class CmdSunModified: public QUndoCommand
{
public:
    CmdSunModified(SunKit* sun, SceneModel* model, QUndoCommand* parent = 0);
    ~CmdSunModified();

     void undo();
     void redo();

     void set(SunKit* sun);

private:
     SunKit* m_sunOld;
     SunKit* m_sun;
     SceneModel* m_model;
};
