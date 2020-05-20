#pragma once
#include <QUndoCommand>

class TShapeKit;
class QModelIndex;
class SoBaseKit;
class SceneModel;

//!  CmdInsertShapeKit class is the insert command for tshapekit nodes  stored in the command stack.
/*!
  CmdInsertShapeKit represents a single tshapekit insertion action on a scene, insert a new tshapekit node to the scene.
*/

class CmdInsertShapeKit: public QUndoCommand
{
public:
    CmdInsertShapeKit(
        TShapeKit* node,
        const QModelIndex& parentIndex,
        SceneModel* model,
        QUndoCommand* parent = 0
     );
    ~CmdInsertShapeKit();

    void undo();
    void redo();

private:
    TShapeKit* m_node;
    SoBaseKit* m_nodeParent;
    SceneModel* m_model;
    int m_row;
};
