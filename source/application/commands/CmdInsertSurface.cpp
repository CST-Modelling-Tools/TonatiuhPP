#include "CmdInsertSurface.h"

#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeRT.h"
#include "kernel/profiles/ProfileRT.h"
#include "kernel/material/MaterialRT.h"
#include "libraries/math/gcf.h"
#include "tree/SceneModel.h"

#include <Inventor/nodes/SoMaterial.h>


CmdInsertSurface::CmdInsertSurface(
    TShapeKit* kit, SoNode* node,
    SceneModel* model, QUndoCommand* parent
):
    QUndoCommand(parent),
    m_kit(kit),
    m_node(node),
    m_model(model)
{
    if (!m_kit || !m_node)
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
//        if (material->getTypeName() == QString("Specular")) {
//            SoMaterial* mg = (SoMaterial*) m_kit->getPart("material", true);
//            mg->diffuseColor = SbVec3f(0.6, 0.7, 0.9);
//            mg->specularColor = SbVec3f(0.3, 0.3, 0.3);
//            mg->shininess = 0.8;
//        }
    }

    m_nodeOld->ref();
    m_node->ref();
    setText(text);
}

CmdInsertSurface::~CmdInsertSurface()
{
    m_node->unref();
    m_nodeOld->unref();
}

void CmdInsertSurface::undo()
{
    set(m_nodeOld);
}

void CmdInsertSurface::redo()
{
    set(m_node);
}

void CmdInsertSurface::set(SoNode* node)
{
    if (dynamic_cast<ShapeRT*>(node))
    {
        m_kit->shapeRT = node;
    }
    else if (dynamic_cast<ProfileRT*>(node))
    {
        m_kit->profileRT = node;
    }
    else if (dynamic_cast<MaterialRT*>(node))
    {
        m_kit->materialRT = node;
    }
}
