#pragma once

#include <QUndoCommand>

class SceneModel;
class SoBaseKit;
class TNode;


class CmdNodeModified: public QUndoCommand
{
public:
    CmdNodeModified(SoBaseKit* kit, QString field, TNode* node, QUndoCommand* parent = 0);
    ~CmdNodeModified();

    void undo();
    void redo();

private:
    void set(TNode* node);

private:
    SoBaseKit* m_kit;
    QString m_field;
    TNode* m_node;
    TNode* m_nodeOld;
};
