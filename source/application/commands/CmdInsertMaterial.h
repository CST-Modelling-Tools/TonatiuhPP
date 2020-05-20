#pragma once

#include <QUndoCommand>

class SceneModel;
class TShapeKit;
class MaterialAbstract;

//! CmdInsertMaterial class is the insert command for materials stored in the command stack.
/*!
   CmdInsertShape represents a single material insertion action on a scene, insert a new material node to the scene.
 */

class CmdInsertMaterial: public QUndoCommand
{
public:
    CmdInsertMaterial(TShapeKit* shapeKit, MaterialAbstract* material, SceneModel* model, QUndoCommand* parent = 0);
    ~CmdInsertMaterial();

    void undo();
    void redo();

private:
    TShapeKit* m_shapeKit;
    MaterialAbstract* m_material;
    SceneModel* m_model;
    int m_row;
};
