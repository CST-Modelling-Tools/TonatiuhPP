#include "CmdInsertShape.h"

#include "kernel/run/InstanceNode.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeRT.h"
#include "libraries/geometry/gcf.h"
#include "tree/SceneModel.h"

/**
 * Creates a new shape insert command that adds a \a shape to \a shapekit node in the \a model.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdInsertShape::CmdInsertShape(TShapeKit* shapeKit, ShapeRT* shape, SceneModel* model, QUndoCommand* parent):
    QUndoCommand("InsertShape", parent),
    m_shapeKit(shapeKit),
    m_shape(shape),
    m_model(model)
{
    if (!m_shapeKit) gcf::SevereError("CmdInsertShape called with NULL TShapeKit*");
    if (!m_shape) gcf::SevereError("CmdInsertShape called with NULL TShape*");

    m_shape->ref();

    m_shapeOld = (ShapeRT*) m_shapeKit->getPart("shape", false);
    m_shapeOld->ref();

    QString text = QString("Create Shape: %1").arg(shape->getTypeName());
    setText(text);
}

CmdInsertShape::~CmdInsertShape()
{
    m_shape->unref();
    m_shapeOld->unref();
}

/*!
 * Reverts model state. After undo() is called, the \a shapekit node will not contain a shape type node.
 * \sa redo().
 */
void CmdInsertShape::undo()
{
    m_shapeKit->setPart("shape", m_shapeOld);
    m_model->replaceCoinNode(m_shapeKit, InstanceNode::IndexShapeRT, m_shapeOld);
}

/*!
 * Applies a change to the model. After redo() the model will contain new shape node located as \a shapekit type node child.
 * \sa undo().
 */
void CmdInsertShape::redo()
{
    m_shapeKit->setPart("shape", m_shape);
    m_model->replaceCoinNode(m_shapeKit, InstanceNode::IndexShapeRT, m_shape);
}
