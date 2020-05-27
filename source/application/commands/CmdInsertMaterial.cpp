#include "CmdInsertMaterial.h"

#include "kernel/material/MaterialRT.h"
#include "kernel/run/InstanceNode.h"
#include "kernel/scene/TShapeKit.h"
#include "libraries/geometry/gcf.h"
#include "tree/SceneModel.h"

/**
 * Creates a new material insert command that adds a \a material to \a shapekit node in the \a model.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdInsertMaterial::CmdInsertMaterial(TShapeKit* shapeKit, MaterialRT* material, SceneModel* model, QUndoCommand* parent):
    QUndoCommand(parent),
    m_shapeKit(shapeKit),
    m_material(material),
    m_model(model)
{
    if (!m_shapeKit) gcf::SevereError("CmdInsertMaterial called with NULL TShapeKit");
    if (!m_material) gcf::SevereError("CmdInsertMaterial called with NULL TMaterial");

    m_material->ref();

    m_materialOld = (MaterialRT*) m_shapeKit->getPart("material", false);
    m_materialOld->ref();

    QString text = QString("Create Material: %1").arg(material->getTypeName());
    setText(text);
}

CmdInsertMaterial::~CmdInsertMaterial()
{
    m_material->unref();
    m_materialOld->unref();
}

/*!
 * Reverts model state. After undo() is called, the shapekit node will not contain a material type node.
 * \sa redo().
 */
void CmdInsertMaterial::undo()
{
    m_shapeKit->setPart("materialRT", m_materialOld);
    m_model->replaceCoinNode(m_shapeKit, InstanceNode::IndexMaterialRT, m_materialOld);
}

/*!
 * Applies a change to the model. After redo() the model will contain new material node located as shapekit type node child.
 * \sa undo().
 */
void CmdInsertMaterial::redo()
{
    m_shapeKit->setPart("materialRT", m_material);
    m_model->replaceCoinNode(m_shapeKit, InstanceNode::IndexMaterialRT, m_material);
}
