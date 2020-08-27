#pragma once

#include <QUndoCommand>

class SceneModel;
class SoBaseKit;
class SoNode;


class CmdInsertSurface: public QUndoCommand
{
public:
    CmdInsertSurface(
        SoBaseKit* kit, SoNode* node,
        SceneModel* model, QUndoCommand* parent = 0
    );
    ~CmdInsertSurface();

    void undo();
    void redo();

private:
    void set(SoNode* node);

private:
    SoBaseKit* m_kit;
    SoNode* m_node;
    SoNode* m_nodeOld;
    SceneModel* m_model;
};
