#include "PhotonMap.h"
#include "PhotonExport.h"

/*!
 * Creates a photon map
 */
PhotonMap::PhotonMap():
    m_bufferSize(0),
    m_pExportPhotonMap(0),
    m_pSceneModel(0),
    m_storedPhotonsInBuffer(0),
    m_storedAllPhotons(0)
{

}

PhotonMap::~PhotonMap()
{
    uint photonListSize = m_photonsInMemory.size();
    for (uint i = 0; i < photonListSize; ++i) {
        delete m_photonsInMemory[i];
        m_photonsInMemory[i] = 0;
    }

    m_photonsInMemory.clear();
    std::vector<Photon*>(m_photonsInMemory).swap(m_photonsInMemory);
    m_storedPhotonsInBuffer = 0;
}

/*!
 * Checks where the photon map has to be saved and saves them.
 */
void PhotonMap::EndStore(double wPhoton)
{
    if (m_storedPhotonsInBuffer > 0)
    {
        if (m_pExportPhotonMap) m_pExportPhotonMap->SavePhotonMap(m_photonsInMemory);

        unsigned int photonListSize = m_photonsInMemory.size();
        for (unsigned int i = 0; i < photonListSize; ++i)
        {
            delete m_photonsInMemory[i];
            m_photonsInMemory[i] = 0;
        }

        m_photonsInMemory.clear();
        std::vector< Photon* >(m_photonsInMemory).swap(m_photonsInMemory);
        m_storedPhotonsInBuffer = 0;

    }
    if (m_pExportPhotonMap) m_pExportPhotonMap->SetPowerPerPhoton(wPhoton);
    if (m_pExportPhotonMap) m_pExportPhotonMap->EndExport();
}

/*!
 *
 */
std::vector<Photon*> PhotonMap::GetAllPhotons() const
{
    return m_photonsInMemory;
}

/*!
 * Returns the export mode. If not export mode defined return null.
 */
PhotonExport* PhotonMap::GetExportMode() const
{
    if (!m_pExportPhotonMap) return 0;
    return (m_pExportPhotonMap);
}

/*!
 * Sets the size of the buffer to \a nPhotons.
 */
void PhotonMap::SetBufferSize(ulong nPhotons)
{
    m_bufferSize = nPhotons;
}

/*!
 * Sets the transformation to change from concentrator coordinates to world coordinates.
 */
void PhotonMap::SetConcentratorToWorld(Transform concentratorToWorld)
{
    m_concentratorToWorld = concentratorToWorld;
    if (m_pExportPhotonMap) m_pExportPhotonMap->SetConcentratorToWorld(m_concentratorToWorld);
}

/*!
 * Sets the photonmap export mode.
 */
bool PhotonMap::SetExportMode(PhotonExport* pExportPhotonMap)
{
    if (!pExportPhotonMap) return 0;
    m_pExportPhotonMap = pExportPhotonMap;
    m_pExportPhotonMap->SetConcentratorToWorld(m_concentratorToWorld);

    if (!m_pExportPhotonMap->StartExport() ) return 0;

    return 1;
}

void PhotonMap::StoreRays(std::vector<Photon>& raysList)
{
    uint raysListSize = raysList.size();
    if (m_storedPhotonsInBuffer > 0 &&  m_storedPhotonsInBuffer + raysListSize > m_bufferSize)
    {
        if (m_pExportPhotonMap)
            m_pExportPhotonMap->SavePhotonMap(m_photonsInMemory);

        for (uint n = 0; n < m_photonsInMemory.size(); ++n)
            delete m_photonsInMemory[n];

        m_photonsInMemory.clear();
        m_storedPhotonsInBuffer = 0;
    }

//    uint nMax = std::min(uint(m_bufferSize), raysListSize);
    for (uint photon = 0; photon < raysListSize; photon++)
        m_photonsInMemory.push_back(new Photon(raysList[photon]) );

    m_storedPhotonsInBuffer += raysListSize;
    m_storedAllPhotons += raysListSize;
}
