#include "Photons.h"
#include "PhotonsAbstract.h"

/*!
 * Creates a photon map
 */
Photons::Photons():
    m_bufferSize(0),
    m_exporter(0),
    m_photonsTotal(0)
{

}

/*!
 * Sets the photonmap export mode.
 */
bool Photons::setExporter(PhotonsAbstract* exporter)
{
    if (!exporter) return false;
    m_exporter = exporter;
    m_exporter->SetConcentratorToWorld(m_transform);
    return m_exporter->StartExport();
}

/*!
 * Sets the transformation to change from concentrator coordinates to world coordinates.
 */
void Photons::setTransform(Transform concentratorToWorld)
{
    m_transform = concentratorToWorld;
    if (m_exporter) m_exporter->SetConcentratorToWorld(m_transform);
}

void Photons::addPhotons(std::vector<Photon>& photons)
{
    uint nMax = photons.size();
    if (m_photons.size() > 0 && m_photons.size() + nMax > m_bufferSize)
    {
        if (m_exporter)
            m_exporter->SavePhotonMap(m_photons);

        m_photons.clear();
    }

//    uint nMax = std::min(uint(m_bufferSize), raysListSize);
    for (uint n = 0; n < nMax; n++)
        m_photons.push_back(photons[n]);

    m_photonsTotal += nMax;
}

/*!
 * Checks where the photon map has to be saved and saves them.
 */
void Photons::endExport(double wPhoton)
{
    if (m_photons.size() > 0)
    {
        if (m_exporter)
            m_exporter->SavePhotonMap(m_photons);

        m_photons.clear();
    }
    if (m_exporter)
    {
        m_exporter->SetPowerPerPhoton(wPhoton);
        m_exporter->EndExport();
    }
}
