#include "CmdLightPositionModified.h"

#include "kernel/sun/TLightKit.h"
#include "libraries/geometry/gf.h"

/**
 * Creates a new lightKit position command that represents a \a light move with \a parent parent.
 *
 * The new position is defined by \a azimuth and \a zenith in radians.
 */
CmdLightPositionModified::CmdLightPositionModified(TLightKit* light, double azimuth, double zenith, QUndoCommand* parent):
    QUndoCommand("Sun position changed", parent), lightKit(light), m_newAzimuth(azimuth), m_newZenith(zenith)
{
    if (light == 0) gf::SevereError("CmdLinghtPositionModified called with NULL TLightKit");

//    m_oldAzimuth = light->azimuth.getValue(); // remove these cmds
//    m_oldZenith = light->zenith.getValue();
}

/*!
 * Destroys the CmdLightPositionModified object.
 */
CmdLightPositionModified::~CmdLightPositionModified()
{
}

/*!
 * Reverts to the previous light positions. After undo() is called, the light position will be the same as before redo() was called.
 *
 * \sa redo().
 */
void CmdLightPositionModified::undo()
{
    lightKit->ChangePosition(m_oldAzimuth, m_oldZenith);
}

/*!
 * Applies a change to the scene. After redo() the light position will be the position defined by the constructor parameters.
 * \sa undo().
 */
void CmdLightPositionModified::redo()
{
    lightKit->ChangePosition(m_newAzimuth, m_newZenith);
}
