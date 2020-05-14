#include "libraries/geometry/gcf.h"

#include "CmdInsertShape.h"
#include "tree/SceneModel.h"
#include "kernel/shape/ShapeAbstract.h"
#include "kernel/scene/TShapeKit.h"

/**
 * Creates a new shape insert command that adds a \a shape to \a shapekit node in the \a model.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdInsertShape::CmdInsertShape(TShapeKit* shapeKit, ShapeAbstract* shape, SceneModel* model, QUndoCommand* parent):
    QUndoCommand("InsertShape", parent), m_shapeKit(shapeKit), m_shape(shape), m_pModel(model), m_row(-1)
{
    if (!m_shapeKit) gcf::SevereError("CmdInsertShape called with NULL TShapeKit*");
    if (!m_shape) gcf::SevereError("CmdInsertShape called with NULL TShape*");
    m_shape->ref();
}

/*!
 * Destroys the CmdInsertShape object.
 */
CmdInsertShape::~CmdInsertShape()
{
    m_shape->unref();
}

/*!
 * Reverts model state. After undo() is called, the \a shapekit node will not contain a shape type node.
 * \sa redo().
 */
void CmdInsertShape::undo()
{
    m_shapeKit->setPart("shape", NULL);
    m_pModel->RemoveCoinNode(m_row, *m_shapeKit);
}

/*!
 * Applies a change to the model. After redo() the model will contain new shape node located as \a shapekit type node child.
 * \sa undo().
 */
void CmdInsertShape::redo()
{
    m_shapeKit->setPart("shape", m_shape);
    m_row = m_pModel->InsertCoinNode(*m_shape, *m_shapeKit);
}
