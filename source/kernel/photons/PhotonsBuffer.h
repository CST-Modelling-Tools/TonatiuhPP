#pragma once

#include "Photon.h"

class PhotonsAbstract;


class TONATIUH_KERNEL PhotonsBuffer
{
public:
    PhotonsBuffer(ulong size);

    void addPhotons(const std::vector<Photon>& photons);
    const std::vector<Photon>& getPhotons() const {return m_photons;} // for drawing rays and flux
    void endExport(double p);

    bool setExporter(PhotonsAbstract* exporter);
    PhotonsAbstract* getExporter() const {return m_exporter;}

private:
    std::vector<Photon> m_photons; // buffer, std is faster than QVector
    ulong m_bufferSize;
    ulong m_photonsTotal;

    PhotonsAbstract* m_exporter;
};
