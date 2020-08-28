#pragma once

#include <QUndoCommand>

class SceneModel;
class SoBaseKit;
class TNode;


class CmdInsertSurface: public QUndoCommand
{
public:
    CmdInsertSurface(SoBaseKit* kit, TNode* node, QUndoCommand* parent = 0);
    ~CmdInsertSurface();

    void undo();
    void redo();

private:
    void set(TNode* node);

private:
    SoBaseKit* m_kit;
    TNode* m_node;
    TNode* m_nodeOld;
};
