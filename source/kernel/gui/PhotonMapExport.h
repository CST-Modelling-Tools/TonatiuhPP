#pragma once

#include <vector>

#include <QStringList>

#include "kernel/raytracing/Photon.h"

class SceneModel;

class PhotonMapExport
{

public:
    PhotonMapExport();
    virtual ~PhotonMapExport() {
    }

    virtual void EndExport() = 0;
    virtual void SavePhotonMap(std::vector<Photon*> raysLists) = 0;
    void SetConcentratorToWorld(Transform concentratorToWorld);
    virtual void SetPowerPerPhoton(double wPhoton) = 0;

    void SetSaveAllPhotonsEnabled();
    void SetSaveCoordinatesEnabled(bool enabled);
    void SetSaveCoordinatesInGlobalSystemEnabled(bool enabled);
    virtual void SetSaveParameterValue(QString parameterName, QString parameterValue) = 0;
    void SetSavePreviousNextPhotonsID(bool enabled);
    void SetSaveSideEnabled(bool enabled);
    void SetSaveSurfacesIDEnabled(bool enabled);
    void SetSaveSurfacesURLList(QStringList surfacesURLList);
    void SetSceneModel(SceneModel& sceneModel);
    virtual bool StartExport() = 0;

protected:
    Transform m_concentratorToWorld;
    SceneModel* m_pSceneModel;
    bool m_saveAllPhotonsData;
    bool m_saveCoordinates;
    bool m_saveCoordinatesInGlobal;
    bool m_savePowerPerPhoton;
    bool m_savePrevNexID;
    bool m_saveSide;
    bool m_saveSurfaceID;
    QStringList m_saveSurfacesURLList;

};
