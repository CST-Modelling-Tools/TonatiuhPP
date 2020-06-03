#include "CmdInsertProfile.h"

#include "kernel/run/InstanceNode.h"
#include "kernel/scene/TShapeKit.h"
#include "kernel/profiles/ProfileRT.h"
#include "libraries/geometry/gcf.h"
#include "tree/SceneModel.h"

/**
 * Creates a new shape insert command that adds a \a shape to \a shapekit node in the \a model.
 *
 * If \a parent is not null, this command is appended to parent's child list and then owns this command.
 */
CmdInsertProfile::CmdInsertProfile(TShapeKit* shapeKit, ProfileRT* profile, SceneModel* model, QUndoCommand* parent):
    QUndoCommand(parent),
    m_shapeKit(shapeKit),
    m_profile(profile),
    m_model(model)
{
    if (!m_shapeKit) gcf::SevereError("CmdInsertProfile called with NULL TShapeKit*");
    if (!m_profile) gcf::SevereError("CmdInsertProfile called with NULL TShape*");

    m_profile->ref();

    m_profileOld = (ProfileRT*) m_shapeKit->profileRT.getValue();
    m_profileOld->ref();

    QString text = QString("Create Profile: %1").arg(profile->getTypeName());
    setText(text);
}

CmdInsertProfile::~CmdInsertProfile()
{
    m_profile->unref();
    m_profileOld->unref();
}

/*!
 * Reverts model state. After undo() is called, the \a shapekit node will not contain a shape type node.
 * \sa redo().
 */
void CmdInsertProfile::undo()
{
    m_shapeKit->profileRT = m_profileOld;
    m_model->replaceCoinNode(m_shapeKit, InstanceNode::IndexProfileRT, m_profileOld);
}

/*!
 * Applies a change to the model. After redo() the model will contain new shape node located as \a shapekit type node child.
 * \sa undo().
 */
void CmdInsertProfile::redo()
{
    m_shapeKit->profileRT = m_profile;
    m_model->replaceCoinNode(m_shapeKit, InstanceNode::IndexProfileRT, m_profile);
}
