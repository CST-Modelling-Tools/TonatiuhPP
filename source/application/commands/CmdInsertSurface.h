#pragma once

#include <QUndoCommand>

class SceneModel;
class TShapeKit;
class SoNode;

//!  CmdInsertShape class is the insert command for shapes stored in the command stack.
/*!
   CmdInsertShape represents a single shape insertion action on a scene, insert a new shape node to the scene.
 */

class CmdInsertSurface: public QUndoCommand
{
public:
    CmdInsertSurface(TShapeKit* kit,
        SoNode* node,
        SceneModel* model,
        QUndoCommand* parent = 0
    );
    ~CmdInsertSurface();

    void undo();
    void redo();

private:
    TShapeKit* m_kit;
    SoNode* m_nodeOld;
    SoNode* m_node;
    SceneModel* m_model;
};
