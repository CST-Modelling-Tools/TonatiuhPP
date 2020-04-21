#pragma once

#include <QUndoCommand>

class SceneModel;
class TShapeKit;
class TMaterial;

//! CmdInsertMaterial class is the insert command for materials stored in the command stack.
/*!
   CmdInsertShape represents a single material insertion action on a scene, insert a new material node to the scene.
 */

class CmdInsertMaterial: public QUndoCommand
{
public:
    CmdInsertMaterial(TShapeKit* shapeKit, TMaterial* material, SceneModel* model, QUndoCommand* parent = 0);
    ~CmdInsertMaterial();

    virtual void undo();
    virtual void redo();

private:
    TShapeKit* m_shapeKit;
    TMaterial* m_material;
    SceneModel* m_pModel;
    int m_row;
};
