#pragma once

#include <QUndoCommand>

class SceneModel;
class TShapeKit;
class SoNode;


class CmdInsertSurface: public QUndoCommand
{
public:
    CmdInsertSurface(
        TShapeKit* kit, SoNode* node,
        SceneModel* model, QUndoCommand* parent = 0
    );
    ~CmdInsertSurface();

    void undo();
    void redo();

private:
    void set(SoNode* node);

private:
    TShapeKit* m_kit;
    SoNode* m_node;
    SoNode* m_nodeOld;
    SceneModel* m_model;
};
