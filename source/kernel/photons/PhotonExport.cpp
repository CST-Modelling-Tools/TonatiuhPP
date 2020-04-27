#include "PhotonExport.h"

/*!
 * Creates an object to export selected data of the photon map .
 */
PhotonExport::PhotonExport():
    m_pSceneModel(0),
    m_saveAllPhotonsData(true),
    m_saveCoordinates(false),
    m_saveCoordinatesInGlobal(true),
    m_savePowerPerPhoton(false),
    m_savePrevNexID(false),
    m_saveSide(false),
    m_saveSurfaceID(false)
{

}

/*!
 * Sets the transformation to change from concentrator coordinates to world coordinates.
 */
void PhotonExport::SetConcentratorToWorld(Transform concentratorToWorld)
{
    m_concentratorToWorld = concentratorToWorld;
}

/*!
 * Sets to store all photons selected data.
 */
void PhotonExport::SetSaveAllPhotonsEnabled()
{
    m_saveAllPhotonsData = true;
    m_saveSurfacesURLList.clear();
}

/*!
 * Sets enabled to save photons intersection coordinates.
 */
void PhotonExport::SetSaveCoordinatesEnabled(bool enabled)
{
    m_saveCoordinates = enabled;
}

/*!
 * Export photons coordinates system into scene global system if \a enabled is true.
 * Otherwise, exports into surface local system.
 */
void PhotonExport::SetSaveCoordinatesInGlobalSystemEnabled(bool enabled)
{
    m_saveCoordinatesInGlobal = enabled;
}

/*!
 * If \a enabled is true, the identifier of the previous and next photons will be exported.
 */
void PhotonExport::SetSavePreviousNextPhotonsID(bool enabled)
{
    m_savePrevNexID = enabled;
}

/*!
 * Sets enabled to save the side of the intersection with the surface.
 */
void PhotonExport::SetSaveSideEnabled(bool enabled)
{
    m_saveSide = enabled;
}

/*!
 * Sets enabled to save the surface identifier.
 */
void PhotonExport::SetSaveSurfacesIDEnabled(bool enabled)
{
    m_saveSurfaceID = enabled;
}

/*!
 *    Sets the list of the surfaces url to save.
 */
void PhotonExport::SetSaveSurfacesURLList(QStringList surfacesURLList)
{
    m_saveAllPhotonsData = false;
    m_saveSurfacesURLList = surfacesURLList;
}

/*!
 * Sets the sceneModel to export mode.
 */
void PhotonExport::SetSceneModel(SceneModel& sceneModel)
{
    m_pSceneModel = &sceneModel;
}
