#pragma once

#include <QStringList>

#include "kernel/scene/TAbstract.h"
#include "Photon.h"

struct PhotonsSettings;

class TONATIUH_KERNEL PhotonsAbstract
{
public:
    PhotonsAbstract();
    virtual ~PhotonsAbstract() {}

    virtual bool startExport() {return true;}
    virtual void savePhotons(const std::vector<Photon>& /*photons*/) {}
    virtual void setPhotonPower(double /*p*/) {}
    virtual void endExport() {}

    void setSceneModel(SceneModel& sceneModel) {m_sceneModel = &sceneModel;}
    void setPhotonSettings(PhotonsSettings* ps);

    static QStringList getParameterNames() {return QStringList();}
    virtual void setParameter(QString /*name*/, QString /*value*/) {}

    NAME_ICON_FUNCTIONS("No export", ":/images/PhotonsDefault.png")

protected:
    SceneModel* m_sceneModel;

    bool m_saveAllPhotonsData;
    QStringList m_surfaces;

    bool m_saveCoordinates;
    bool m_saveCoordinatesGlobal;
    bool m_saveSurfaceID;
    bool m_saveSurfaceSide;
    bool m_savePhotonsID;
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
