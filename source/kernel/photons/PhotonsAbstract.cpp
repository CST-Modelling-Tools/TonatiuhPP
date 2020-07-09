#include "PhotonsAbstract.h"

#include "PhotonsSettings.h"

PhotonsAbstract::PhotonsAbstract():
    m_sceneModel(0),
    m_saveCoordinates(false),
    m_saveCoordinatesGlobal(true),
    m_saveSurfaceID(false),
    m_saveSurfaceSide(false),
    m_savePhotonsID(false)
{

}

void PhotonsAbstract::setPhotonSettings(PhotonsSettings* ps)
{
    m_surfaces = ps->surfaces;

    m_saveCoordinates = ps->saveCoordinates;
    m_saveCoordinatesGlobal = ps->saveCoordinatesGlobal;
    m_saveSurfaceID = ps->saveSurfaceID;
    m_saveSurfaceSide = ps->saveSurfaceSide;
    m_savePhotonsID = ps->savePhotonsID;

    QMap<QString, QString>::const_iterator it;
    for (it = ps->parameters.begin(); it != ps->parameters.end(); it++)
        setParameter(it.key(), it.value());
}
