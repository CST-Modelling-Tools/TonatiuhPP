#include "CmdNodeModified.h"

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

CmdNodeModified::CmdNodeModified(SoBaseKit* kit, QString field, TNode* node, QUndoCommand* parent):
    QUndoCommand(parent),
    m_kit(kit),
    m_field(field),
    m_node(node)
{
    if (!m_kit || !m_node)
        gcf::SevereError("CmdInsertShape called with NULL TShapeKit*");

    SoSFNode* sf = (SoSFNode*) m_kit->getField(m_field.toLatin1().data());
    m_nodeOld = (TNode*) sf->getValue();

    m_nodeOld->ref();
    m_node->ref();

    QString text = QString("\"%1\" = %2").arg(m_field, m_node->getTypeName());
    setText(text);
}

CmdNodeModified::~CmdNodeModified()
{
    m_node->unref();
    m_nodeOld->unref();
}

void CmdNodeModified::undo()
{
    set(m_nodeOld);
}

void CmdNodeModified::redo()
{
    set(m_node);
}

void CmdNodeModified::set(TNode* node)
{
    SoField* f = m_kit->getField(m_field.toLatin1().data());

    if (f)
        static_cast<SoSFNode*>(f)->setValue(node);
}



//        if (material->getTypeName() == QString("Specular")) {
//            SoMaterial* mg = (SoMaterial*) m_kit->getPart("material", true);
//            mg->diffuseColor = SbVec3f(0.6, 0.7, 0.9);
//            mg->specularColor = SbVec3f(0.3, 0.3, 0.3);
//            mg->shininess = 0.8;
//        }
