#pragma once

#include <vector>

#include <QStringList>

#include "Photon.h"

class SceneModel;

class TONATIUH_KERNEL PhotonsAbstract
{
public:
    PhotonsAbstract();
    virtual ~PhotonsAbstract() {}

    virtual void EndExport() {}
    virtual void SavePhotonMap(std::vector<Photon*> /*raysLists*/) {}
    void SetConcentratorToWorld(Transform concentratorToWorld);
    virtual void SetPowerPerPhoton(double /*wPhoton*/) {}

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

    static const char* getClassName() {return "PhotonMapExport";}
    static const char* getClassIcon() {return ":/PhotonMapExport.png";}
    const char* getIcon() const {return getClassIcon();}

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
