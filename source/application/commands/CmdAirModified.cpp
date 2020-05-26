#include "CmdAirModified.h"

#include "kernel/scene/TSceneKit.h"
#include "kernel/air/Air.h"


CmdAirModified::CmdAirModified(Air* air, TSceneKit* sceneKit, QUndoCommand* parent):
    QUndoCommand("Air modified", parent),
    m_airOld(0),
    m_air(0),
    m_sceneKit(sceneKit)
{
    Air* airOld = dynamic_cast<Air*>(m_sceneKit->getPart("air", false));
    if (airOld) {
        m_airOld = airOld;
        m_airOld->ref();
    }

    if (air) {
        m_air = static_cast<Air*>(air->copy(true));
        m_air->ref();
    }
}

CmdAirModified::~CmdAirModified()
{
    m_airOld->unref();
    m_air->unref();
}

/*!
 * Reverts to the previous transmissivity definition. After undo() is called, the scene transmissivityn will be the same as before redo() was called.
 *
 * \sa redo().
 */
void CmdAirModified::undo()
{
    m_sceneKit->setPart("air", m_airOld);
}

/*!
 * Applies a change to the scene. After redo() the transmissivity will be new transmissivity defined by the constructor parameters.
 * \sa undo().
 */
void CmdAirModified::redo()
{
    m_sceneKit->setPart("air", m_air);
}
