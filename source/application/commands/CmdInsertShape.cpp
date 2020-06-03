#include "CmdInsertShape.h"

#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeRT.h"
#include "kernel/profiles/ProfileRT.h"
#include "kernel/material/MaterialRT.h"
#include "libraries/geometry/gcf.h"
#include "tree/SceneModel.h"

/**
 * Creates a new shape insert command that adds a \a shape to \a shapekit node in the \a model.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdInsertShape::CmdInsertShape(
    TShapeKit* kit,
    SoNode* node,
    SceneModel* model,
    QUndoCommand* parent
):
    QUndoCommand(parent),
    m_kit(kit),
    m_node(node),
    m_model(model)
{
    if (!m_kit || ! node)
        gcf::SevereError("CmdInsertShape called with NULL TShapeKit*");

    QString text;
    if (ShapeRT* shape = dynamic_cast<ShapeRT*>(node))
    {
        m_nodeOld = m_kit->shapeRT.getValue();
        text = QString("Create Shape: %1").arg(shape->getTypeName());
    }
    else if (ProfileRT* profile = dynamic_cast<ProfileRT*>(node))
    {
        m_nodeOld = m_kit->profileRT.getValue();
        text = QString("Create Profile: %1").arg(profile->getTypeName());
    }
    else if (MaterialRT* material = dynamic_cast<MaterialRT*>(node))
    {
        m_nodeOld = m_kit->materialRT.getValue();
        text = QString("Create Material: %1").arg(material->getTypeName());
    }

    m_nodeOld->ref();
    m_node->ref();
    setText(text);
}

CmdInsertShape::~CmdInsertShape()
{
    m_node->unref();
    m_nodeOld->unref();
}

/*!
 * Reverts model state. After undo() is called, the \a shapekit node will not contain a shape type node.
 * \sa redo().
 */
void CmdInsertShape::undo()
{
    m_model->replaceCoinNode(m_kit, m_nodeOld);
}

/*!
 * Applies a change to the model. After redo() the model will contain new shape node located as \a shapekit type node child.
 * \sa undo().
 */
void CmdInsertShape::redo()
{
    m_model->replaceCoinNode(m_kit, m_node);
}
