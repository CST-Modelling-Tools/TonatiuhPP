#pragma once

#include "Photon.h"

class PhotonsAbstract;


class TONATIUH_KERNEL Photons
{
public:
    Photons();

    bool setExporter(PhotonsAbstract* exporter);
    PhotonsAbstract* getExporter() const {return m_exporter;}
    void setBufferSize(ulong size) {m_bufferSize = size;}
    void setTransform(Transform concentratorToWorld);

    void addPhotons(std::vector<Photon>& photons);
    const std::vector<Photon>& getPhotons() const {return m_photons;}
    void endExport(double wPhoton);

private:
    PhotonsAbstract* m_exporter;
    ulong m_bufferSize;
    Transform m_transform;
    std::vector<Photon> m_photons; // buffer
    ulong m_photonsTotal;
};
