#include "libraries/geometry/gf.h"

#include "CmdAirModified.h"
#include "kernel/raytracing/TSceneKit.h"
#include "kernel/air/AirAbstract.h"

/*!
 * Creates a new transmissivity definition command. The
 */
CmdAirModified::CmdAirModified(AirAbstract* airNew, TSceneKit* scene, QUndoCommand* parent):
    QUndoCommand("Transmissivity changed", parent),
    m_hasOld(false),
    m_airNew(0),
    m_scene(scene)
{
    if (airNew) {
        m_airNew = static_cast<AirAbstract*>(airNew->copy(true) );
        m_airNew->ref();
    }

    SoNode* node = m_scene->getPart("transmissivity", false);
    if (node) {
        m_hasOld = true;
        m_airOld = dynamic_cast<AirAbstract*>(node);
        if (m_airOld) m_airOld->ref();
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
    if (m_hasOld) m_scene->setPart("transmissivity", m_airOld);
    else m_scene->setPart("transmissivity", 0);
}

/*!
 * Applies a change to the scene. After redo() the transmissivity will be new transmissivity defined by the constructor parameters.
 * \sa undo().
 */
void CmdAirModified::redo()
{
    if (m_airNew) m_scene->setPart("transmissivity", m_airNew);
    else m_scene->setPart("transmissivity", 0);
}
