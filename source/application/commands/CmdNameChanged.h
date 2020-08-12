#pragma once

#include <QUndoCommand>

class QString;
class SoNode;
class SceneModel;


class CmdNameChanged: public QUndoCommand
{
public:
    CmdNameChanged(const QModelIndex& index, QString name, SceneModel* model, QUndoCommand* parent = 0);
    ~CmdNameChanged() {}

    void undo();
    void redo();

private:
    QString m_name;
    QString m_nameOld;
    SoNode* m_node;
    SceneModel* m_model;
};
