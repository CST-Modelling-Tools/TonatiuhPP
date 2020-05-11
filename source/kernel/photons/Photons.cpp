#include "Photons.h"
#include "PhotonsAbstract.h"

/*!
 * Creates a photon map
 */
Photons::Photons():
    m_bufferSize(0),
    m_exporter(0),
//    m_sceneModel(0),
    m_photonsBuffered(0),
    m_photonsTotal(0)
{

}

Photons::~Photons()
{
    for (Photon* p : m_photons)
        delete p;
}

/*!
 * Returns the export mode. If not export mode defined return null.
 */
PhotonsAbstract* Photons::getExporter() const
{
    if (!m_exporter) return 0;
    return m_exporter;
}

/*!
 * Sets the photonmap export mode.
 */
bool Photons::setExporter(PhotonsAbstract* exporter)
{
    if (!exporter) return 0;
    m_exporter = exporter;
    m_exporter->SetConcentratorToWorld(m_transform);

    if (!m_exporter->StartExport()) return false;
    return true;
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
    if (m_photonsBuffered > 0 && m_photonsBuffered + nMax > m_bufferSize)
    {
        if (m_exporter)
            m_exporter->SavePhotonMap(m_photons);

        for (Photon* p : m_photons)
            delete p;
        m_photons.clear();
        m_photonsBuffered = 0;
    }

//    uint nMax = std::min(uint(m_bufferSize), raysListSize);
    for (uint n = 0; n < nMax; n++)
        m_photons.push_back(new Photon(photons[n]) );

    m_photonsBuffered += nMax;
    m_photonsTotal += nMax;
}

/*!
 * Checks where the photon map has to be saved and saves them.
 */
void Photons::endExport(double wPhoton)
{
    if (m_photonsBuffered > 0)
    {
        if (m_exporter)
            m_exporter->SavePhotonMap(m_photons);

        for (Photon* p : m_photons)
            delete p;
        m_photons.clear();
        m_photonsBuffered = 0;
    }
    if (m_exporter)
    {
        m_exporter->SetPowerPerPhoton(wPhoton);
        m_exporter->EndExport();
    }
}
