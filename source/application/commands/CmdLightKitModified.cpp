#include "CmdLightKitModified.h"

#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>
#include "libraries/geometry/gf.h"
#include "gui/SceneModel.h"
#include "kernel/sun/TLightKit.h"

/**
 * Creates a new lightKit modification command that represents a new light defined as \a newLightKit to the \a scene.
 *
 * If the model has not previous light a light node is added to \a sceneModel.
 */

CmdLightKitModified::CmdLightKitModified(
    TLightKit* lightKitNew,
    SoSceneKit* sceneKit,
    SceneModel& sceneModel,
    QUndoCommand* parent
):
    QUndoCommand("Modify LightKit", parent),
    m_hasOld(false),
    m_lightKitNew(0),
    m_sunShapeOld(0),
    m_azimuthOld(0),
    m_zenithOld(0),
    m_nodesOld(""),
    m_sceneKit(sceneKit),
    m_sceneModel(&sceneModel)
{
    if (!lightKitNew)
        gf::SevereError("CmdLightKitModified called with NULL TLightKit*");

    m_lightKitNew = static_cast<TLightKit*>(lightKitNew->copy(true));
    m_lightKitNew->ref();

    TLightKit* lightKit = dynamic_cast<TLightKit*>(m_sceneKit->getPart("lightList[0]", false));
    if (lightKit) {
        m_hasOld = true;
        m_sunShapeOld = dynamic_cast<SunAbstract*>(lightKit->getPart("tsunshape", false)->copy(true) );
        if (m_sunShapeOld) m_sunShapeOld->ref();
        m_azimuthOld = lightKit->azimuth.getValue();
        m_zenithOld = lightKit->zenith.getValue();
        m_nodesOld = lightKit->disabledNodes.getValue().getString();
    }
}

/*!
 * Destroys the CmdLightKitModified object.
 */
CmdLightKitModified::~CmdLightKitModified()
{
    m_lightKitNew->unref();
    m_sunShapeOld->unref();
}

/*!
 * Reverts to the previous light. After undo() is called, the state of the scene will be the same as before redo() was called.
 *  * \sa redo().
 */
void CmdLightKitModified::undo()
{
    if (m_hasOld) {
        TLightKit* lightKit = static_cast<TLightKit*> (m_sceneKit->getPart("lightList[0]", false) );
        lightKit->setPart("tsunshape", m_sunShapeOld);
        lightKit->ChangePosition(m_azimuthOld, m_zenithOld);
        lightKit->disabledNodes.setValue(m_nodesOld.toStdString().c_str() );
    } else
        m_sceneModel->RemoveLightNode(*m_lightKitNew);
}

/*!
 * Applies a change to the light. After redo() scene will contain the light with the new definition.
 * \sa undo().
 */
void CmdLightKitModified::redo()
{
    if (m_hasOld) {
        TLightKit* lightKit = static_cast<TLightKit*>(m_sceneKit->getPart("lightList[0]", false));

        SunAbstract* shape = static_cast<SunAbstract*>(m_lightKitNew->getPart("tsunshape", false));
        lightKit->setPart("tsunshape", shape);

        lightKit->ChangePosition(m_lightKitNew->azimuth.getValue(), m_lightKitNew->zenith.getValue());
        lightKit->disabledNodes.setValue(m_lightKitNew->disabledNodes.getValue() );
    } else
        m_sceneModel->InsertLightNode(*m_lightKitNew);
}
