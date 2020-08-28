#include "CmdInsertSurface.h"

#include "kernel/scene/TShapeKit.h"
#include "kernel/shape/ShapeRT.h"
#include "kernel/profiles/ProfileRT.h"
#include "kernel/material/MaterialRT.h"
#include "kernel/trackers/TrackerArmature.h"
#include "kernel/air/AirTransmission.h"
#include "kernel/sun/SunShape.h"
#include "libraries/math/gcf.h"
#include "tree/SceneModel.h"

#include <Inventor/nodes/SoMaterial.h>

// todo: add parameter name

CmdInsertSurface::CmdInsertSurface(SoBaseKit* kit, TNode* node, QUndoCommand* parent):
    QUndoCommand(parent),
    m_kit(kit),
    m_node(node)
{
    if (!m_kit || !m_node)
        gcf::SevereError("CmdInsertShape called with NULL TShapeKit*");

    QString text;
    if (dynamic_cast<ShapeRT*>(node))
    {
        SoSFNode* sf = (SoSFNode*) m_kit->getField("shapeRT");
        m_nodeOld = (TNode*) sf->getValue();
        text = QString("Create Shape: %1").arg(node->getTypeName());
    }
    else if (dynamic_cast<ProfileRT*>(node))
    {
        SoSFNode* sf = (SoSFNode*) m_kit->getField("profileRT");
        m_nodeOld = (TNode*) sf->getValue();
        text = QString("Create Profile: %1").arg(node->getTypeName());
    }
    else if (dynamic_cast<MaterialRT*>(node))
    {
        SoSFNode* sf = (SoSFNode*) m_kit->getField("materialRT");
        m_nodeOld = (TNode*) sf->getValue();
        text = QString("Create Material: %1").arg(node->getTypeName());
    }
    else if (dynamic_cast<TrackerArmature*>(node))
    {
        SoSFNode* sf = (SoSFNode*) m_kit->getField("armature");
        m_nodeOld = (TNode*) sf->getValue();
        text = QString("Create Armature: %1").arg(node->getTypeName());
    }
    else if (dynamic_cast<AirTransmission*>(node))
    {
        SoSFNode* sf = (SoSFNode*) m_kit->getField("transmission");
        m_nodeOld = (TNode*) sf->getValue();
        text = QString("Create Air: %1").arg(node->getTypeName());
    }
    else if (dynamic_cast<SunShape*>(node))
    {
        SoSFNode* sf = (SoSFNode*) m_kit->getField("shape");
        m_nodeOld = (TNode*) sf->getValue();
        text = QString("Create Sun: %1").arg(node->getTypeName());
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

void CmdInsertSurface::set(TNode* node)
{
    SoField* f = 0;
    if (dynamic_cast<ShapeRT*>(node))
        f = m_kit->getField("shapeRT");
    else if (dynamic_cast<ProfileRT*>(node))
        f = m_kit->getField("profileRT");
    else if (dynamic_cast<MaterialRT*>(node))
        f = m_kit->getField("materialRT");
    else if (dynamic_cast<TrackerArmature*>(node))
        f = m_kit->getField("armature");
    else if (dynamic_cast<AirTransmission*>(node))
        f = m_kit->getField("transmission");
    else if (dynamic_cast<SunShape*>(node))
        f = m_kit->getField("shape");

    if (f)
        static_cast<SoSFNode*>(f)->setValue(node);
}



//        if (material->getTypeName() == QString("Specular")) {
//            SoMaterial* mg = (SoMaterial*) m_kit->getPart("material", true);
//            mg->diffuseColor = SbVec3f(0.6, 0.7, 0.9);
//            mg->specularColor = SbVec3f(0.3, 0.3, 0.3);
//            mg->shininess = 0.8;
//        }
