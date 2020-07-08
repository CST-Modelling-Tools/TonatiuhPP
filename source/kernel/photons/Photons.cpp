#include "Photons.h"
#include "PhotonsAbstract.h"

/*!
 * Creates a photon map
 */
Photons::Photons():
    m_exporter(0),
    m_bufferSize(0),
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
    m_exporter->setTransform(m_transform);
    return m_exporter->startExport();
}

/*!
 * Sets the transformation to change from concentrator coordinates to world coordinates.
 */
void Photons::setTransform(Transform concentratorToWorld)
{
    m_transform = concentratorToWorld;
    if (m_exporter) m_exporter->setTransform(m_transform);
}

void Photons::addPhotons(const std::vector<Photon>& photons)
{
    uint nMax = photons.size();
    if (m_photons.size() > 0 && m_photons.size() + nMax > m_bufferSize)
    {
        if (m_exporter)
            m_exporter->savePhotons(m_photons);

        m_photons.clear();
    }

//    uint nMax = std::min(uint(m_bufferSize), raysListSize);
//    for (uint n = 0; n < nMax; n++)
//        m_photons.push_back(photons[n]);
    m_photons.insert(m_photons.end(), photons.begin(), photons.end());

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
            m_exporter->savePhotons(m_photons);

        m_photons.clear();
    }
    if (m_exporter)
    {
        m_exporter->setPhotonPower(wPhoton);
        m_exporter->endExport();
    }
}
