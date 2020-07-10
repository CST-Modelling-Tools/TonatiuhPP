#include "PhotonsBuffer.h"
#include "PhotonsAbstract.h"

PhotonsBuffer::PhotonsBuffer(ulong size):
    m_photonsMax(size),
    m_exporter(0)
{
    m_photons.reserve(size);
}

void PhotonsBuffer::addPhotons(const std::vector<Photon>& photons)
{
    uint nMax = photons.size();
    if (m_photons.size() > 0 && m_photons.size() + nMax > m_photonsMax)
    {
        if (m_exporter) m_exporter->savePhotons(m_photons);
        m_photons.clear();
    }

    m_photons.insert(m_photons.end(), photons.begin(), photons.end());
}

void PhotonsBuffer::endExport(double p)
{
    if (m_photons.size() > 0)
    {
        if (m_exporter) m_exporter->savePhotons(m_photons);
        m_photons.clear();
    }
    if (m_exporter)
    {
        m_exporter->setPhotonPower(p);
        m_exporter->endExport();
    }
}

bool PhotonsBuffer::setExporter(PhotonsAbstract* exporter)
{
    if (!exporter) return false;
    m_exporter = exporter;
    return m_exporter->startExport();
}
