#pragma once

#include <QUndoCommand>

class SoNode;
class SceneModel;


class CmdParameterModified: public QUndoCommand
{

public:
    CmdParameterModified(
        SoNode* node, QString name, QString value,
        SceneModel* model, QUndoCommand* parent = 0
    );

    void undo();
    void redo();

private:
    SoNode* m_node;
    QString m_parameter;
    QString m_value;
    QString m_valueOld;
    SceneModel* m_model;
};
