#pragma once

#include <QUndoCommand>

class QString;
class SceneModel;
class SoNode;

class CmdChangeNodeName: public QUndoCommand
{
public:
    CmdChangeNodeName(const QModelIndex& index, QString newName, SceneModel* model, QUndoCommand* parent = 0);
    ~CmdChangeNodeName() {}

    virtual void undo();
    virtual void redo();

private:
    QString m_newName;/*!< New node name*/
    QString m_previousName;/*!< Previous node name*/
    SoNode* m_pNode; /*!< Changed node*/
    SceneModel* m_pModel; /*!< The scene */
};
