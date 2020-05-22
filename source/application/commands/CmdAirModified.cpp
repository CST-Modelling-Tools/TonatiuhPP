#include "libraries/geometry/gcf.h"

#include "CmdAirModified.h"
#include "kernel/scene/TSceneKit.h"
#include "kernel/air/Air.h"

/*!
 * Creates a new transmissivity definition command. The
 */
CmdAirModified::CmdAirModified(Air* air, TSceneKit* sceneKit, QUndoCommand* parent):
    QUndoCommand("Air changed", parent),
    m_airOld(0),
    m_air(0),
    m_sceneKit(sceneKit)
{
    if (air) {
        m_air = static_cast<Air*>(air->copy(true));
        m_air->ref();
    }

    SoNode* node = m_sceneKit->getPart("air", false);
    if (m_airOld = dynamic_cast<Air*>(node)) {
        m_airOld->ref();
    }
}

/*!
 * Destroys the CmdLightPositionModified object.
 */
CmdAirModified::~CmdAirModified()
{
    //if( m_pNewTransmissivity && m_pNewTransmissivity->getRefCount() > 0 )     m_pNewTransmissivity->unref();
    //if( m_pPreviousTransmissivity && m_pPreviousTransmissivity->getRefCount() > 0 ) m_pPreviousTransmissivity->unref();
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
