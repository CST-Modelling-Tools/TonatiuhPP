#pragma once

#include <vector>

#include <QStringList>

#include "kernel/scene/TAbstract.h"
#include "Photon.h"

class SceneModel;


class TONATIUH_KERNEL PhotonsAbstract
{
public:
    PhotonsAbstract();
    virtual ~PhotonsAbstract() {}

    virtual bool StartExport() {return true;}
    virtual void EndExport() {}
    virtual void SavePhotonMap(const std::vector<Photon>& /*raysLists*/) {}
    virtual void SetPowerPerPhoton(double /*wPhoton*/) {}

    void SetSceneModel(SceneModel& sceneModel) {m_sceneModel = &sceneModel;}
    void SetConcentratorToWorld(Transform transform) {m_transform = transform;}

    void SetSaveAllPhotonsEnabled();
    void SetSaveSurfacesURLList(QStringList surfaces);

    void SetSaveCoordinates(bool enabled);
    void SetSaveCoordinatesInGlobalSystem(bool enabled);
    void SetSavePreviousNextPhotonsID(bool enabled);
    void SetSaveSide(bool enabled);
    void SetSaveSurfacesID(bool enabled);

    static QStringList GetParameterNames() {return QStringList();}
    virtual void SetSaveParameterValue(QString /*name*/, QString /*value*/) {}

    NAME_ICON_FUNCTIONS("No export", ":/images/PhotonsDefault.png")

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



#include "kernel/scene/TFactory.h"

class PhotonsAbstract;
class PhotonsWidget;

class TONATIUH_KERNEL PhotonsFactory: public TFactory
{
public:
    virtual PhotonsAbstract* create() const = 0;
    virtual PhotonsWidget* createWidget() const {return 0;}
};

Q_DECLARE_INTERFACE(PhotonsFactory, "tonatiuh.PhotonsFactory")



template<class T, class W>
class PhotonsFactoryT: public PhotonsFactory
{
public:
    QString name() const {return T::getClassName();}
    QIcon icon() const {return QIcon(T::getClassIcon());}
    T* create() const {return new T;}
    W* createWidget() const {return new W;}
};
