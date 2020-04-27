#pragma once

#include "Photon.h"

class PhotonExport;

class TONATIUH_KERNEL PhotonMap
{
public:
    PhotonMap();
    ~PhotonMap();

    void EndStore(double wPhoton);
    std::vector<Photon*> GetAllPhotons() const;
    PhotonExport* GetExportMode() const;
    void SetBufferSize(ulong nPhotons);
    void SetConcentratorToWorld(Transform concentratorToWorld);
    bool SetExportMode(PhotonExport* pExportPhotonMap);
    void StoreRays(std::vector<Photon>& ray);

private:
    ulong m_bufferSize;
    Transform m_concentratorToWorld;
    PhotonExport* m_pExportPhotonMap;
    const SceneModel* m_pSceneModel;
    unsigned long m_storedPhotonsInBuffer;
    unsigned long m_storedAllPhotons;
    std::vector<Photon*> m_photonsInMemory;
};
