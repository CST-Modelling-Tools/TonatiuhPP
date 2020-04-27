#pragma once

#include <QMap>
#include <QString>

#include "kernel/photons/PhotonExport.h"

class Photon;

class PhotonExportNull: public PhotonExport
{

public:
    PhotonExportNull() {}

    static QStringList GetParameterNames() {return QStringList();}
    void SetSaveParameterValue(QString /*parameterName*/, QString /*parameterValue*/) {}
    bool StartExport() {return true;}

    static const char* getClassName() {return "No export";}
    static const char* getClassIcon() {return ":/PhotonExportNull.png";}
    const char* getIcon() const {return getClassIcon();}
};



#include "kernel/photons/PhotonExportFactory.h"
#include "kernel/photons/PhotonExportWidget.h"

class PhotonExportNullFactory:
    public QObject,
    public PhotonExportFactoryT<PhotonExportNull, PhotonExportWidget>
{
    Q_OBJECT
    Q_INTERFACES(PhotonExportFactory)
    Q_PLUGIN_METADATA(IID "tonatiuh.PhotonExportFactory")
};
