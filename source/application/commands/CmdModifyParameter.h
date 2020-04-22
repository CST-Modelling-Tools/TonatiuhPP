#pragma once

#include <QUndoCommand>

class SceneModel;
class SoNode;

class CmdModifyParameter : public QUndoCommand
{

public:
    CmdModifyParameter( SoNode* node, QString parameterName, QString parameterValue, SceneModel* model, QUndoCommand* parent = 0 );
    ~CmdModifyParameter();

     virtual void undo();
     virtual void redo();

private:
     SoNode* m_coinNode;
    QString m_newValue;
    QString m_oldValue;
    QString m_parameterName;
    SceneModel* m_pModel;

};
