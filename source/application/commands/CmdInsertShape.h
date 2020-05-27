#pragma once

#include <QUndoCommand>

class SceneModel;
class TShapeKit;
class ShapeRT;

//!  CmdInsertShape class is the insert command for shapes stored in the command stack.
/*!
   CmdInsertShape represents a single shape insertion action on a scene, insert a new shape node to the scene.
 */

class CmdInsertShape: public QUndoCommand
{
public:
    CmdInsertShape(TShapeKit* shapeKit, ShapeRT* shape, SceneModel* model, QUndoCommand* parent = 0);
    ~CmdInsertShape();

    void undo();
    void redo();

private:
    TShapeKit* m_shapeKit;
    ShapeRT* m_shapeOld;
    ShapeRT* m_shape;
    SceneModel* m_model;
};
