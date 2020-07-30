#include "CmdLightPositionModified.h"

#include "kernel/sun/SunKit.h"
#include "libraries/math/gcf.h"

/**
 * Creates a new lightKit position command that represents a \a light move with \a parent parent.
 *
 * The new position is defined by \a azimuth and \a zenith in radians.
 */
CmdLightPositionModified::CmdLightPositionModified(SunKit* light, double azimuth, double zenith, QUndoCommand* parent):
    QUndoCommand("Sun position changed", parent), sunKit(light), m_azimuth(azimuth), m_elevation(zenith)
{
    if (light == 0) gcf::SevereError("CmdLinghtPositionModified called with NULL SunKit");

//    m_oldAzimuth = light->azimuth.getValue(); // remove these cmds
//    m_oldZenith = light->zenith.getValue();
}

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
    sunKit->azimuth.setValue(m_azimuthOld);
    sunKit->elevation.setValue(m_elevationOld);
    sunKit->updateTransform();
}

/*!
 * Applies a change to the scene. After redo() the light position will be the position defined by the constructor parameters.
 * \sa undo().
 */
void CmdLightPositionModified::redo()
{
    sunKit->azimuth.setValue(m_azimuth);
    sunKit->elevation.setValue(m_elevation);
    sunKit->updateTransform();
}
