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

    virtual bool StartExport() = 0;
    virtual void EndExport() {}
    virtual void SavePhotonMap(const std::vector<Photon>& /*raysLists*/) {}

    void SetSceneModel(SceneModel& sceneModel) {m_sceneModel = &sceneModel;}
    void SetConcentratorToWorld(Transform transform) {m_transform = transform;}

    void SetSaveAllPhotonsEnabled();
    void SetSaveSurfacesURLList(QStringList surfaces);

    void SetSaveCoordinates(bool enabled);
    void SetSaveCoordinatesInGlobalSystem(bool enabled);
    void SetSavePreviousNextPhotonsID(bool enabled);
    void SetSaveSide(bool enabled);
    void SetSaveSurfacesID(bool enabled);

    virtual void SetPowerPerPhoton(double /*wPhoton*/) {}
    virtual void SetSaveParameterValue(QString name, QString value) = 0;

    static const char* getClassName() {return "PhotonMapExport";}
    static const char* getClassIcon() {return ":/PhotonMapExport.png";}
    const char* getIcon() const {return getClassIcon();}

protected:
    SceneModel* m_sceneModel;
    Transform m_transform;
    bool m_saveAllPhotonsData;
    bool m_saveCoordinates;
    bool m_saveCoordinatesInGlobal;
    bool m_savePowerPerPhoton;
    bool m_savePrevNextID;
    bool m_saveSide;
    bool m_saveSurfaceID;
    QStringList m_saveSurfaces;
};
