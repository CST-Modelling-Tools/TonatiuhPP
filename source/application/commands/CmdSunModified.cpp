#include "CmdSunModified.h"

#include <Inventor/nodekits/SoSceneKit.h>

#include "libraries/math/gcf.h"
#include "tree/SceneModel.h"
#include "kernel/scene/TSceneKit.h"
#include "kernel/sun/SunKit.h"

/**
 * Creates a new lightKit modification command that represents a new light defined as \a newLightKit to the \a scene.
 *
 * If the model has not previous light a light node is added to \a sceneModel.
 */
CmdSunModified::CmdSunModified(SunKit* sunKit,
    TSceneKit* sceneKit,
    SceneModel* model,
    QUndoCommand* parent
):
    QUndoCommand("Sun modified", parent),
    m_sunKitOld(0),
    m_sunKit(0),
    m_sceneKit(sceneKit),
    m_model(model)
{
    SunKit* sunKitOld = dynamic_cast<SunKit*>(m_sceneKit->getPart("lightList[0]", false));
    if (sunKitOld) {
        m_sunKitOld = sunKitOld;
        m_sunKitOld->ref();
    }

    if (sunKit) {
        m_sunKit = static_cast<SunKit*>(sunKit->copy(true));
        m_sunKit->ref();
    }
}

CmdSunModified::~CmdSunModified()
{
    m_sunKitOld->unref();
    m_sunKit->unref();
}

/*!
 * Reverts to the previous light. After undo() is called, the state of the scene will be the same as before redo() was called.
 *  * \sa redo().
 */
void CmdSunModified::undo()
{
    m_model->insertSunNode(m_sunKitOld);
}

/*!
 * Applies a change to the light. After redo() scene will contain the light with the new definition.
 * \sa undo().
 */
void CmdSunModified::redo()
{
    m_model->insertSunNode(m_sunKit);
}
