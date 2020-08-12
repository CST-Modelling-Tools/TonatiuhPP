#pragma once

#include <QUndoCommand>

class SunKitW;
class SceneModel;

class CmdSunModified: public QUndoCommand
{
public:
    CmdSunModified(SunKitW* sun, SceneModel* model, QUndoCommand* parent = 0);
    ~CmdSunModified();

     void undo();
     void redo();

     void set(SunKitW* sun);

private:
     SunKitW* m_sunOld;
     SunKitW* m_sun;
     SceneModel* m_model;
};
