#pragma once

#include "Photon.h"

class PhotonsAbstract;

class TONATIUH_KERNEL Photons
{
public:
    Photons();
    ~Photons();

    PhotonsAbstract* getExporter() const;
    bool setExporter(PhotonsAbstract* exporter);
    void setBufferSize(ulong size) {m_bufferSize = size;}
    void setTransform(Transform concentratorToWorld);

    void addPhotons(std::vector<Photon>& photons);
    void endExport(double wPhoton);
    const std::vector<Photon*>& getPhotons() const {return m_photons;}

private:
    ulong m_bufferSize;
    Transform m_transform;
    PhotonsAbstract* m_exporter;
//    const SceneModel* m_sceneModel;
    ulong m_photonsBuffered;
    ulong m_photonsTotal;
    std::vector<Photon*> m_photons; // buffer
};
