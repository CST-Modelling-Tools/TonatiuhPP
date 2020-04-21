#pragma once
#include <QUndoCommand>

class QModelIndex;
class TShapeKit;
class SceneModel;

//!  CmdInsertShapeKit class is the insert command for tshapekit nodes  stored in the command stack.
/*!
  CmdInsertShapeKit represents a single tshapekit insertion action on a scene, insert a new tshapekit node to the scene.
*/

class CmdInsertShapeKit : public QUndoCommand
{
public:
    CmdInsertShapeKit( const QModelIndex& parentIndex, TShapeKit* shapeKit, SceneModel* model, QUndoCommand* parent = 0 );
    ~CmdInsertShapeKit();

     virtual void undo();
     virtual void redo();

 private:
        SoBaseKit* m_coinParent;
     TShapeKit* m_shapeKit;
     SceneModel* m_pModel;
     int m_row;

};
