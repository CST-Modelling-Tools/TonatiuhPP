#include "PhotonsAbstract.h"

/*!
 * Creates an object to export selected data of the photon map
 */
PhotonsAbstract::PhotonsAbstract():
    m_sceneModel(0),
    m_saveAllPhotonsData(true),
    m_saveCoordinates(false),
    m_saveCoordinatesInGlobal(true),
    m_savePowerPerPhoton(false),
    m_savePrevNextID(false),
    m_saveSide(false),
    m_saveSurfaceID(false)
{

}

/*!
 * Sets to store all photons selected data.
 */
void PhotonsAbstract::SetSaveAllPhotonsEnabled()
{
    m_saveAllPhotonsData = true;
    m_saveSurfaces.clear();
}

/*!
 *    Sets the list of the surfaces url to save.
 */
void PhotonsAbstract::SetSaveSurfacesURLList(QStringList surfaces)
{
    m_saveAllPhotonsData = false;
    m_saveSurfaces = surfaces;
}

/*!
 * Sets enabled to save photons intersection coordinates.
 */
void PhotonsAbstract::SetSaveCoordinates(bool enabled)
{
    m_saveCoordinates = enabled;
}

/*!
 * Export photons coordinates system into scene global system if \a enabled is true.
 * Otherwise, exports into surface local system.
 */
void PhotonsAbstract::SetSaveCoordinatesInGlobalSystem(bool enabled)
{
    m_saveCoordinatesInGlobal = enabled;
}

/*!
 * If \a enabled is true, the identifier of the previous and next photons will be exported.
 */
void PhotonsAbstract::SetSavePreviousNextPhotonsID(bool enabled)
{
    m_savePrevNextID = enabled;
}

/*!
 * Sets enabled to save the side of the intersection with the surface.
 */
void PhotonsAbstract::SetSaveSide(bool enabled)
{
    m_saveSide = enabled;
}

/*!
 * Sets enabled to save the surface identifier.
 */
void PhotonsAbstract::SetSaveSurfacesID(bool enabled)
{
    m_saveSurfaceID = enabled;
}
