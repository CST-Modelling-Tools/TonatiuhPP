#pragma once

#include <QUndoCommand>

class SceneModel;
class TShapeKit;
class TShape;

//!  CmdInsertShape class is the insert command for shapes stored in the command stack.
/*!
   CmdInsertShape represents a single shape insertion action on a scene, insert a new shape node to the scene.
 */

class CmdInsertShape: public QUndoCommand
{
public:
    CmdInsertShape(TShapeKit* shapeKit, TShape* shape, SceneModel* model, QUndoCommand* parent = 0);
    ~CmdInsertShape();

    virtual void undo();
    virtual void redo();

private:
    TShapeKit* m_shapeKit;
    TShape* m_shape;
    SceneModel* m_pModel;
    int m_row;
};
