#pragma once

#include <QUndoCommand>

class SceneModel;
class SoNode;

class CmdParameterModified: public QUndoCommand
{

public:
    CmdParameterModified(SoNode* node, QString name, QString value, SceneModel* model, QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    SoNode* m_coinNode;
    QString m_newValue;
    QString m_oldValue;
    QString m_parameterName;
    SceneModel* m_pModel;
};
